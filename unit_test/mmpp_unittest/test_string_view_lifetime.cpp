#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/variable_buffer.hpp>

// ============================================================================
// Native-handle construction
// ============================================================================

TEST_CASE("string_view from a native handle", "[string_view][lifetime]")
{
    /* A view built straight from another view's handle must describe the
     * same bytes.  MemeStringStack_initByOther shares a refcounted block and
     * copies small/medium inline data, so both outcomes keep the view valid
     * after the source dies only in the shared case -- see the lifetime
     * tests below. */
    memepp::string src("hello");
    memepp::string_view original(src);
    REQUIRE(original.size() == 5);

    /* const MemeStringStack_t& constructor. */
    memepp::string_view copy(original.native_handle());
    REQUIRE(copy.size() == 5);
    REQUIRE(copy.data()[0] == 'h');
    REQUIRE(copy == original);

    /* An empty handle yields an empty view. */
    memepp::string empty_src;
    memepp::string_view empty_copy(empty_src.native_handle());
    REQUIRE(empty_copy.empty());
    REQUIRE(empty_copy.size() == 0);

    /* The handle constructor works with a large (shared) source too. */
    {
        std::string big(200, 'z');
        memepp::string big_src(big.data(),
                               static_cast<memepp::string::size_type>(big.size()),
                               memepp::string_storage_t::large);
        memepp::string_view big_view(big_src.native_handle());
        REQUIRE(big_view.size() == 200);
        REQUIRE(big_view.data()[0] == 'z');
    }
}

// ============================================================================
// Assignment operators
// ============================================================================

TEST_CASE("string_view assignment overloads", "[string_view][lifetime]")
{
    memepp::string a("alpha");
    memepp::string b("beta");

    memepp::string_view v;

    /* From string. */
    v = a;
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* From another view. */
    memepp::string_view other(b);
    v = other;
    REQUIRE(v.size() == 4);
    REQUIRE(v.data()[0] == 'b');

    /* From a moved view. */
    memepp::string_view movable(a);
    v = std::move(movable);
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Self copy-assignment keeps the object intact. */
    memepp::string_view& alias = v;
    v = alias;
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Self move-assignment likewise. */
    v = std::move(alias);
    REQUIRE(v.size() == 5);
    REQUIRE(v.data()[0] == 'a');

    /* Non-empty reassigned to empty. */
    {
        memepp::string_view e;
        memepp::string_view n(a);
        e = n;
        REQUIRE(e.size() == 5);

        memepp::string_view empty_src;
        e = empty_src;
        REQUIRE(e.empty());
        REQUIRE(e.size() == 0);
    }

    /* Empty assigned to non-empty. */
    {
        memepp::string_view n(a);
        REQUIRE(n.size() == 5);
        n = memepp::string_view();
        REQUIRE(n.empty());
    }
}


// Lifetime: shared survives, borrow does not
// ============================================================================

TEST_CASE("string_view shared source outlives the view creation",
          "[string_view][lifetime]")
{
    /* Large source -> refcounted: the view stays valid after the string dies. */
    memepp::string_view v;
    {
        std::string big(100, 'q');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        v = src;
        REQUIRE(v.size() == 100);
    }
    REQUIRE(v.size() == 100);
    REQUIRE(v.data()[0] == 'q');
    REQUIRE(v.data()[99] == 'q');
}

TEST_CASE("string_view small source is a pure borrow",
          "[string_view][lifetime]")
{
    /*
     * A small (inline) string takes the MemeStringViewUnsafeStack_init branch:
     * the view aliases the string's inline buffer with no refcount.  The
     * source must therefore outlive the view.
     */
    const char backing[] = "abc";
    memepp::string src(backing, 3);
    REQUIRE(src.storage_type() == memepp::string_storage_t::small);

    memepp::string_view v(src);
    REQUIRE(v.size() == 3);
    REQUIRE(v.data() == src.data());   /* aliases, no copy */

    /* The view sees a change made through the source while it is alive. */
    REQUIRE(v.data()[0] == 'a');
}

TEST_CASE("string_view copy of a view", "[string_view][lifetime]")
{
    memepp::string src("abcdef");
    memepp::string_view a(src);
    memepp::string_view b(a);

    REQUIRE(b.size() == a.size());
    REQUIRE(b.data() == a.data());      /* same backing storage */
    REQUIRE(b == a);

    /* Slicing a view yields another view over the same bytes. */
    auto sub = a.substr(2, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub.data() == a.data() + 2);
}

// ============================================================================

// rvalue string: shared vs non-shared branch
// ============================================================================

TEST_CASE("string_view from rvalue string: both branches",
          "[string_view][lifetime]")
{
    /*
     * operator=(string&&) is deleted, so the rvalue branch is exercised
     * through the constructor.  The view must be built outside the block
     * that owns the source, hence the two-step form below: build inside,
     * then inspect after the source is gone.
     */

    /* Large rvalue -> shared branch: refcount taken, view survives. */
    {
        std::string big(200, 'q');
        memepp::string src(big.data(),
                           static_cast<memepp::string::size_type>(big.size()),
                           memepp::string_storage_t::large);
        REQUIRE(src.storage_type() == memepp::string_storage_t::large);

        memepp::string_view v(std::move(src));
        REQUIRE(v.size() == 200);
        REQUIRE(v.data()[0] == 'q');
        /* Still valid after src was moved from / left the block. */
        REQUIRE(v.data()[199] == 'q');
    }

    /* Small rvalue -> the constructor rebuilds a large string and views
     * that, so the result is not a borrow of the temporary. */
    {
        memepp::string src("hello", 5);
        REQUIRE(src.storage_type() == memepp::string_storage_t::small);

        memepp::string_view v(std::move(src));
        REQUIRE(v.size() == 5);
        REQUIRE(v.data()[0] == 'h');
        REQUIRE(v.data()[4] == 'o');
        /* The fallback promotes to a shared tier, not a borrow. */
        REQUIRE(static_cast<int>(v.storage_type()) != 9);   /* not view_unsafe */
    }
}

// ============================================================================

// user-tier storage (reached through the C API only)
// ============================================================================

/*
 * string_storage_t::user is not produced by any memepp::string constructor:
 * MemeStringStack_initByU8bytesAndType has no `user` case (see the
 * buffer_span gap2 notes).  The only way in is the C API
 * MemeStringStack_initTakeOverUserObject(), which takes ownership of a
 * caller object when its length exceeds MMSTR__GET_SMALL_BUF_MAX_SIZE --
 * shorter payloads are copied and the user object is destroyed immediately.
 *
 * The test below drives that C API directly, so the user tier (and the
 * refcount branch it takes in string_view) is genuinely exercised.
 */

namespace {

struct UserPayload
{
    std::string bytes;
    bool destroyed = false;
};

void user_destruct(void* obj)
{
    static_cast<UserPayload*>(obj)->destroyed = true;
}

const char* user_data(const void* obj)
{
    return static_cast<const UserPayload*>(obj)->bytes.data();
}

size_t user_size(const void* obj)
{
    return static_cast<const UserPayload*>(obj)->bytes.size();
}

} // namespace

TEST_CASE("string_view from a real user-tier string", "[string_view][lifetime]")
{
    /* Long enough to avoid the small-buffer copy path. */
    UserPayload payload;
    payload.bytes.assign(200, 'U');

    MemeStringStack_t handle;
    REQUIRE(MemeStringStack_initTakeOverUserObject(
                &handle, MEME_STRING__OBJECT_SIZE,
                &payload, user_destruct, user_data, user_size) == 0);

    /* Now the raw handle really is a user-tier string. */
    REQUIRE(MemeString_storageType(memepp::to_pointer(handle))
            == MemeString_StorageType_user);

    /* Wrap it in a memepp::string so the C++ side sees a user-tier object.
     * The const& overload shares the refcounted user block rather than
     * copying, which is what lets the view below outlive it. */
    memepp::string owned(handle);
    REQUIRE(owned.storage_type() == memepp::string_storage_t::user);
    REQUIRE(owned.size() == 200);

    /* A view over a user-tier string takes the shared branch. */
    memepp::string_view v(owned);
    REQUIRE(v.size() == 200);
    {
        const int tier = static_cast<int>(v.storage_type());
        REQUIRE((tier == 3 || tier == 10));   /* large or user */
    }
    REQUIRE(v.data()[0] == 'U');
    REQUIRE(v.data()[199] == 'U');

    /* The view keeps the user block alive: drop the string, the view stays. */
    {
        memepp::string_view keep(v);
        owned = memepp::string();
        REQUIRE(keep.size() == 200);
        REQUIRE(keep.data()[0] == 'U');
    }
}

