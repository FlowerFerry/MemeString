#include <catch2/catch.hpp>

#include <megopp/help/objbuf.h>

#include <cstdint>
#include <cstring>
#include <vector>

struct ObjBufTriv {
    int    a;
    float  b;
    double c;
};
static_assert(std::is_trivially_copyable<ObjBufTriv>::value, "ObjBufTriv must be trivially copyable");

TEST_CASE("mgpp::help::object_buffer - default constructor yields nullptr", "[objbuf]")
{
    mgpp::help::object_buffer<int> buf;
    REQUIRE(buf.get() == nullptr);
}

TEST_CASE("mgpp::help::object_buffer - size constructor with sufficient bytes", "[objbuf]")
{
    mgpp::help::object_buffer<int> buf(sizeof(int));
    REQUIRE(buf.get() != nullptr);
}

TEST_CASE("mgpp::help::object_buffer - size constructor with zero bytes yields nullptr", "[objbuf]")
{
    mgpp::help::object_buffer<int> buf(0);
    REQUIRE(buf.get() == nullptr);
}

TEST_CASE("mgpp::help::object_buffer - size constructor for struct", "[objbuf]")
{
    mgpp::help::object_buffer<ObjBufTriv> buf(sizeof(ObjBufTriv));
    REQUIRE(buf.get() != nullptr);
}

TEST_CASE("mgpp::help::object_buffer - src() is consistent with get()", "[objbuf]")
{
    mgpp::help::object_buffer<uint32_t> buf(sizeof(uint32_t));
    auto& v = buf.src();
    REQUIRE(!v.empty());

    uint32_t val = 0xDEADBEEFu;
    std::memcpy(v.data(), &val, sizeof(val));
    REQUIRE(*buf.get() == val);
}

TEST_CASE("mgpp::help::object_buffer - const src() accessible", "[objbuf]")
{
    const mgpp::help::object_buffer<int> buf(sizeof(int));
    const auto& v = buf.src();
    REQUIRE(!v.empty());
}

TEST_CASE("mgpp::help::object_buffer - move-from-vector constructor", "[objbuf]")
{
    std::vector<uint8_t> v(sizeof(double) * 2, 0xABu);
    mgpp::help::object_buffer<double> buf(std::move(v));
    REQUIRE(buf.get() != nullptr);
}

TEST_CASE("mgpp::help::objbuf alias works identically", "[objbuf]")
{
    mgpp::help::objbuf<int> buf(sizeof(int));
    REQUIRE(buf.get() != nullptr);
}
