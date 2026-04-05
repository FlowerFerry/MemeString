#include <catch2/catch.hpp>

#include <megopp/memory/emtx_cow_ptr.h>
#include <megopp/util/scope_locker.h>

#include <memory>
#include <mutex>
#include <shared_mutex>

namespace {

struct EData {
    int value{0};
};

using Ptr = mgpp::mem::emtx_cow_ptr<EData, std::mutex, std::shared_mutex>;

// ---------------------------------------------------------------------------
// Helpers — wrap the external-mutex locking protocol
// ---------------------------------------------------------------------------

std::shared_ptr<const EData> do_read(Ptr& p, std::shared_mutex& gmtx)
{
    std::shared_lock<std::shared_mutex> sl(gmtx, std::defer_lock);
    return p.read(sl);
}

template <typename Fn>
std::shared_ptr<const EData> do_write(Ptr& p, std::mutex& wmtx, std::shared_mutex& gmtx, Fn&& fn)
{
    std::unique_lock<std::mutex>           w_lk(wmtx, std::defer_lock);
    mgpp::util::rw_lock<std::shared_mutex> g_lk(gmtx, std::defer_lock);
    return p.write(w_lk, g_lk, std::forward<Fn>(fn));
}

template <typename CondFn, typename Fn>
std::shared_ptr<const EData> do_write_if(Ptr& p, std::mutex& wmtx, std::shared_mutex& gmtx,
                                          CondFn&& cond_fn, Fn&& fn)
{
    std::unique_lock<std::mutex>           w_lk(wmtx, std::defer_lock);
    mgpp::util::rw_lock<std::shared_mutex> g_lk(gmtx, std::defer_lock);
    return p.write_if(w_lk, g_lk, std::forward<CondFn>(cond_fn), std::forward<Fn>(fn));
}

} // namespace

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - default construction is null", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    Ptr p;
    REQUIRE(do_read(p, gmtx) == nullptr);
}

TEST_CASE("emtx_cow_ptr - nullptr construction is null", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    Ptr p(nullptr);
    REQUIRE(do_read(p, gmtx) == nullptr);
}

TEST_CASE("emtx_cow_ptr - construct from shared_ptr", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    sp->value = 7;
    Ptr p(sp);
    REQUIRE(do_read(p, gmtx)->value == 7);
}

TEST_CASE("emtx_cow_ptr - construct from unique_ptr", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    auto up = std::make_unique<EData>();
    up->value = 13;
    Ptr p(std::move(up));
    REQUIRE(do_read(p, gmtx)->value == 13);
}

// ---------------------------------------------------------------------------
// write — COW semantics
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - write creates a COW copy and applies the mutation", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    Ptr p(sp);

    do_write(p, wmtx, gmtx, [](std::shared_ptr<EData>& ptr) {
        ptr->value = 99;
    });

    REQUIRE(do_read(p, gmtx)->value == 99);
    REQUIRE(sp->value == 0); // original object was not modified
}

TEST_CASE("emtx_cow_ptr - write with bool fn returning true applies update", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    Ptr p(std::make_shared<EData>());

    do_write(p, wmtx, gmtx, [](std::shared_ptr<EData>& ptr) -> bool {
        ptr->value = 42;
        return true;
    });

    REQUIRE(do_read(p, gmtx)->value == 42);
}

TEST_CASE("emtx_cow_ptr - write with bool fn returning false cancels the update", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    Ptr p(std::make_shared<EData>());
    auto before = do_read(p, gmtx);

    auto result = do_write(p, wmtx, gmtx, [](std::shared_ptr<EData>& ptr) -> bool {
        ptr->value = 99;
        return false;
    });

    REQUIRE(result == before);
    REQUIRE(do_read(p, gmtx)->value == 0);
}

// ---------------------------------------------------------------------------
// write_if
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - write_if skips mutation when condition returns false", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    Ptr p(std::make_shared<EData>());

    do_write_if(p, wmtx, gmtx,
        [](const auto&) -> bool { return false; },
        [](std::shared_ptr<EData>& ptr) { ptr->value = 77; });

    REQUIRE(do_read(p, gmtx)->value == 0);
}

TEST_CASE("emtx_cow_ptr - write_if applies mutation when condition returns true", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    Ptr p(std::make_shared<EData>());

    do_write_if(p, wmtx, gmtx,
        [](const auto&) -> bool { return true; },
        [](std::shared_ptr<EData>& ptr) { ptr->value = 88; });

    REQUIRE(do_read(p, gmtx)->value == 88);
}

TEST_CASE("emtx_cow_ptr - write_if condition receives the current value", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    Ptr p(std::make_shared<EData>());
    do_write(p, wmtx, gmtx, [](std::shared_ptr<EData>& ptr) { ptr->value = 5; });

    // Only update when current value is 5
    do_write_if(p, wmtx, gmtx,
        [](const auto& cur) -> bool { return cur && cur->value == 5; },
        [](std::shared_ptr<EData>& ptr) { ptr->value = 10; });

    REQUIRE(do_read(p, gmtx)->value == 10);
}

// ---------------------------------------------------------------------------
// write_unsafe — direct in-place mutation (no COW copy)
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - write_unsafe mutates shared_ptr in-place without copying", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    Ptr p(sp);

    std::unique_lock<std::mutex>           w_lk(wmtx, std::defer_lock);
    std::unique_lock<std::shared_mutex>    g_lk(gmtx, std::defer_lock);

    p.write_unsafe(w_lk, g_lk, [](std::shared_ptr<EData>& ptr) {
        ptr->value = 55;
    });

    // write_unsafe operates on the same shared_ptr — sp is also modified
    REQUIRE(sp->value == 55);
    REQUIRE(do_read(p, gmtx)->value == 55);
}

TEST_CASE("emtx_cow_ptr - write_unsafe with bool fn returning false skips", "[emtx_cow_ptr]")
{
    std::mutex wmtx;
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    Ptr p(sp);

    std::unique_lock<std::mutex>           w_lk(wmtx, std::defer_lock);
    std::unique_lock<std::shared_mutex>    g_lk(gmtx, std::defer_lock);

    p.write_unsafe(w_lk, g_lk, [](std::shared_ptr<EData>& ptr) -> bool {
        ptr->value = 99;
        return false;
    });

    REQUIRE(sp->value == 99);
    REQUIRE(do_read(p, gmtx)->value == 99);
}

// ---------------------------------------------------------------------------
// assign
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - assign copies value from another instance", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    sp->value = 33;
    Ptr p1;
    Ptr p2(sp);

    std::unique_lock<std::shared_mutex> g_lk(gmtx, std::defer_lock);
    p1.assign(p2, g_lk);

    REQUIRE(do_read(p1, gmtx)->value == 33);
}

TEST_CASE("emtx_cow_ptr - assign self is a no-op", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    sp->value = 7;
    Ptr p(sp);

    std::unique_lock<std::shared_mutex> g_lk(gmtx, std::defer_lock);
    p.assign(p, g_lk);

    REQUIRE(do_read(p, gmtx)->value == 7);
}

// ---------------------------------------------------------------------------
// release
// ---------------------------------------------------------------------------

TEST_CASE("emtx_cow_ptr - release returns the shared_ptr and clears the instance", "[emtx_cow_ptr]")
{
    std::shared_mutex gmtx;
    auto sp = std::make_shared<EData>();
    sp->value = 5;
    Ptr p(sp);

    std::unique_lock<std::shared_mutex> g_lk(gmtx, std::defer_lock);
    auto released = p.release(g_lk);

    REQUIRE(released == sp);
    REQUIRE(do_read(p, gmtx) == nullptr);
}
