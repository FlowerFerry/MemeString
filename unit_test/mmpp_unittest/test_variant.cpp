#include <catch2/catch.hpp>

#include <memepp/variant.hpp>
#include <memepp/string.hpp>
#include <memepp/buffer.hpp>
#include <memepp/variable_buffer.hpp>
#include <memepp/rune.hpp>
#include <memepp/string_view.hpp>
#include <memepp/buffer_view.hpp>

// ---------------------------------------------------------------------------
// default constructor — null variant
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant default constructor is null", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.is_null());
    REQUIRE(!v.is_type(memepp::meta::typid::byte));
}

// ---------------------------------------------------------------------------
// mmbyte_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from mmbyte_t", "[variant]")
{
    memepp::variant v(static_cast<mmbyte_t>(0xAB));
    REQUIRE(!v.is_null());
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    mmbyte_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == static_cast<mmbyte_t>(0xAB));
    REQUIRE(v.get_or<mmbyte_t>() == static_cast<mmbyte_t>(0xAB));
}

// ---------------------------------------------------------------------------
// char
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from char", "[variant]")
{
    memepp::variant v('Z');
    REQUIRE(v.is_type(memepp::meta::typid::char_t));

    char out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 'Z');
    REQUIRE(v.get_or<char>() == 'Z');
}

// ---------------------------------------------------------------------------
// int64_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from int64_t", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(-9876543210LL));
    REQUIRE(v.is_type(memepp::meta::typid::int64));

    int64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == -9876543210LL);
    REQUIRE(v.get_or<int64_t>() == -9876543210LL);
}

// ---------------------------------------------------------------------------
// uint64_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from uint64_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(12345678901234ULL));
    REQUIRE(v.is_type(memepp::meta::typid::uint64));

    uint64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 12345678901234ULL);
}

// ---------------------------------------------------------------------------
// double
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from double", "[variant]")
{
    memepp::variant v(3.14159);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));

    double out = 0.0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == Approx(3.14159));
    REQUIRE(v.get_or<double>() == Approx(3.14159));
}

// ---------------------------------------------------------------------------
// string
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from string", "[variant]")
{
    memepp::string s("hello variant");
    memepp::variant v(s);
    REQUIRE(v.is_type(memepp::meta::typid::string));

    memepp::string out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == "hello variant");
    REQUIRE(v.get_or<memepp::string>() == "hello variant");
}

// ---------------------------------------------------------------------------
// rune
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from rune", "[variant]")
{
    memepp::rune r('A');
    memepp::variant v(r);
    REQUIRE(v.is_type(memepp::meta::typid::rune));

    memepp::rune out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 'A');
}

// ---------------------------------------------------------------------------
// copy & move
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant copy constructor", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(42LL));
    memepp::variant dst(src);

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 42LL);
}

TEST_CASE("memepp::variant move constructor", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(99LL));
    memepp::variant dst(std::move(src));

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 99LL);
}

// ---------------------------------------------------------------------------
// copy & move assignment
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant copy assignment", "[variant]")
{
    memepp::variant src(static_cast<double>(2.71828));
    memepp::variant dst;
    dst = src;

    REQUIRE(dst.is_type(memepp::meta::typid::double_t));
    double out = 0.0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == Approx(2.71828));
}

TEST_CASE("memepp::variant move assignment", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(7LL));
    memepp::variant dst;
    dst = std::move(src);

    REQUIRE(dst.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(dst.try_get(out) == 0);
    REQUIRE(out == 7LL);
}

// ---------------------------------------------------------------------------
// type operator= overloads
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant operator= primitive types", "[variant]")
{
    memepp::variant v;

    v = static_cast<mmbyte_t>(0x01);
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    v = 'X';
    REQUIRE(v.is_type(memepp::meta::typid::char_t));

    v = static_cast<int64_t>(1000LL);
    REQUIRE(v.is_type(memepp::meta::typid::int64));

    v = static_cast<uint64_t>(2000ULL);
    REQUIRE(v.is_type(memepp::meta::typid::uint64));

    v = 1.5;
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant reset", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(123LL));
    REQUIRE(!v.is_null());
    v.reset();
    REQUIRE(v.is_null());
}

// ---------------------------------------------------------------------------
// swap
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant swap", "[variant]")
{
    memepp::variant a(static_cast<int64_t>(1LL));
    memepp::variant b(static_cast<double>(9.9));

    a.swap(b);

    REQUIRE(a.is_type(memepp::meta::typid::double_t));
    REQUIRE(b.is_type(memepp::meta::typid::int64));

    double da = 0.0;
    REQUIRE(a.try_get(da) == 0);
    REQUIRE(da == Approx(9.9));

    int64_t db = 0;
    REQUIRE(b.try_get(db) == 0);
    REQUIRE(db == 1LL);
}

// ---------------------------------------------------------------------------
// try_convert: int64 -> double, double -> bool
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant try_convert int64 to double", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    double out = 0.0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == Approx(42.0));
    REQUIRE(v.convert_or<double>() == Approx(42.0));
}

TEST_CASE("memepp::variant try_convert double to bool", "[variant]")
{
    memepp::variant v_true(1.0);
    bool b = false;
    REQUIRE(v_true.try_convert(b) == 0);
    REQUIRE(b == true);

    memepp::variant v_false(0.0);
    REQUIRE(v_false.try_convert(b) == 0);
    REQUIRE(b == false);
}

TEST_CASE("memepp::variant convert_or returns default on wrong type get", "[variant]")
{
    memepp::variant v('A');

    // try_get for wrong type should fail
    int64_t out = 0;
    REQUIRE(v.try_get(out) != 0);
    REQUIRE(v.get_or<int64_t>(static_cast<int64_t>(-1)) == static_cast<int64_t>(-1));
}

// ---------------------------------------------------------------------------
// set<T> template
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant set<T>", "[variant]")
{
    memepp::variant v;

    REQUIRE(v.set(static_cast<mmbyte_t>(0xFE)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::byte));

    REQUIRE(v.set(static_cast<int64_t>(777LL)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    int64_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == 777LL);
}

// ---------------------------------------------------------------------------
// variant: variable_buffer type
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant — variable_buffer type", "[variant]")
{
    memepp::variable_buffer vb;
    vb.push_back(1);
    vb.push_back(2);
    memepp::variant v = vb;
    REQUIRE(v.is_type(memepp::meta::typid::varbuf));
    REQUIRE(v.type_id() == memepp::meta::typid::varbuf);
    auto val = v.get_or<memepp::variable_buffer>();
    REQUIRE(val.size() == 2);
}

// ---------------------------------------------------------------------------
// variant: reset — double reset
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant reset — double reset", "[variant]")
{
    memepp::variant v;
    v.reset();
    REQUIRE(v.is_null());
}

// ---------------------------------------------------------------------------
// variant: swap — same type
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant swap — same type", "[variant]")
{
    memepp::variant a(static_cast<int64_t>(10LL));
    memepp::variant b(static_cast<int64_t>(20LL));

    a.swap(b);
    REQUIRE(a.get_or<int64_t>() == 20);
    REQUIRE(b.get_or<int64_t>() == 10);
}

// ---------------------------------------------------------------------------
// variant: native_handle (compile check only)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant native_handle compiles", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    const auto& nh = v.native_handle();
    (void)nh;
}

// ---------------------------------------------------------------------------
// variant: assign from wchar_t
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant assign from wchar_t", "[variant]")
{
    memepp::variant v;
    v = wchar_t(L'A');
    REQUIRE(v.is_type(memepp::meta::typid::wchar));
    REQUIRE(v.get_or<wchar_t>() == L'A');
}

// ---------------------------------------------------------------------------
// variant: assign from buffer
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant assign from buffer", "[variant]")
{
    const uint8_t data[] = { 1, 2, 3 };
    memepp::buffer buf(data, static_cast<memepp::buffer::size_type>(3));
    memepp::variant v;
    v = buf;
    REQUIRE(v.is_type(memepp::meta::typid::buffer));
    auto val = v.get_or<memepp::buffer>();
    REQUIRE(val.size() == 3);
}

// ===========================================================================
// below: tests for previously uncovered APIs
// ===========================================================================

// ---------------------------------------------------------------------------
// constructors — missing
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant from wchar_t", "[variant]")
{
    memepp::variant v(wchar_t(L'猫'));
    REQUIRE(v.is_type(memepp::meta::typid::wchar));

    wchar_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == L'猫');
    REQUIRE(v.get_or<wchar_t>() == L'猫');
}

TEST_CASE("memepp::variant from buffer", "[variant]")
{
    const uint8_t raw[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(4));
    memepp::variant v(buf);
    REQUIRE(v.is_type(memepp::meta::typid::buffer));

    memepp::buffer out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out.size() == 4);
    REQUIRE(v.get_or<memepp::buffer>().size() == 4);
}

TEST_CASE("memepp::variant from mmvar_cptr_t", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(42LL));
    const auto& nh = src.native_handle();
    memepp::variant v(memepp::to_pointer(nh));
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    REQUIRE(v.get_or<int64_t>() == 42LL);
}

TEST_CASE("memepp::variant from native_handle_type&&", "[variant]")
{
    memepp::variant::native_handle_type nh;
    MemeVariantStack_initByInt64(&nh, MMVAR__OBJ_SIZE, 999LL);
    memepp::variant v(std::move(nh));
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    REQUIRE(v.get_or<int64_t>() == 999LL);
}

TEST_CASE("memepp::variant from const native_handle_type&", "[variant]")
{
    memepp::variant src(static_cast<double>(3.14));
    const auto& nh = src.native_handle();
    memepp::variant v(nh);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
    REQUIRE(v.get_or<double>() == Approx(3.14));
}

// ---------------------------------------------------------------------------
// operator= — missing
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant operator= string", "[variant]")
{
    memepp::variant v;
    memepp::string s("operator= string");
    v = s;
    REQUIRE(v.is_type(memepp::meta::typid::string));
    REQUIRE(v.get_or<memepp::string>() == "operator= string");
}

TEST_CASE("memepp::variant operator= rune", "[variant]")
{
    memepp::variant v;
    memepp::rune r('R');
    v = r;
    REQUIRE(v.is_type(memepp::meta::typid::rune));
    REQUIRE(v.get_or<memepp::rune>() == 'R');
}

TEST_CASE("memepp::variant operator= variable_buffer", "[variant]")
{
    memepp::variant v;
    memepp::variable_buffer vb;
    vb.push_back(10);
    vb.push_back(20);
    v = vb;
    REQUIRE(v.is_type(memepp::meta::typid::varbuf));
    auto val = v.get_or<memepp::variable_buffer>();
    REQUIRE(val.size() == 2);
}

// ---------------------------------------------------------------------------
// try_get — missing types
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant try_get wchar_t", "[variant]")
{
    memepp::variant v(wchar_t(L'Ω'));
    wchar_t out = 0;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == L'Ω');
}

TEST_CASE("memepp::variant try_get buffer", "[variant]")
{
    const uint8_t raw[] = { 0x01, 0x02 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(2));
    memepp::variant v(buf);
    memepp::buffer out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out.size() == 2);
}

TEST_CASE("memepp::variant try_get variable_buffer", "[variant]")
{
    memepp::variable_buffer vb;
    vb.push_back(7);
    memepp::variant v(vb);
    memepp::variable_buffer out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out.size() == 1);
}

TEST_CASE("memepp::variant try_get string_view", "[variant]")
{
    memepp::string s("view me");
    memepp::variant v(s);
    memepp::string_view out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out == "view me");
}

TEST_CASE("memepp::variant try_get buffer_view", "[variant]")
{
    const uint8_t raw[] = { 0x10, 0x20, 0x30 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(3));
    memepp::variant v(buf);
    memepp::buffer_view out;
    REQUIRE(v.try_get(out) == 0);
    REQUIRE(out.size() == 3);
}

// ---------------------------------------------------------------------------
// try_convert — missing types
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant try_convert to mmint_t", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    mmint_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<mmint_t>(42));
}

TEST_CASE("memepp::variant try_convert to size_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(100ULL));
    size_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<size_t>(100));
}

// ---------------------------------------------------------------------------
// set<T> — missing types
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant set bool", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(true) == 0);
    bool out = false;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == true);
}

TEST_CASE("memepp::variant set char", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set('Q') == 0);
    REQUIRE(v.is_type(memepp::meta::typid::char_t));
    REQUIRE(v.get_or<char>() == 'Q');
}

TEST_CASE("memepp::variant set wchar_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(wchar_t(L'β')) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::wchar));
    REQUIRE(v.get_or<wchar_t>() == L'β');
}

TEST_CASE("memepp::variant set int16_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<int16_t>(-100)) == 0);
    mmint_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<mmint_t>(-100));
}

TEST_CASE("memepp::variant set uint16_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<uint16_t>(200)) == 0);
    size_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<size_t>(200));
}

TEST_CASE("memepp::variant set mmint_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<mmint_t>(12345)) == 0);
    mmint_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<mmint_t>(12345));
}

TEST_CASE("memepp::variant set size_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<size_t>(67890)) == 0);
    size_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<size_t>(67890));
}

TEST_CASE("memepp::variant set uint64_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<uint64_t>(999888777ULL)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::uint64));
    REQUIRE(v.get_or<uint64_t>() == 999888777ULL);
}

TEST_CASE("memepp::variant set float", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(1.5f) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
    REQUIRE(v.get_or<double>() == Approx(1.5));
}

TEST_CASE("memepp::variant set double", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(3.14159) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
    REQUIRE(v.get_or<double>() == Approx(3.14159));
}

TEST_CASE("memepp::variant set string", "[variant]")
{
    memepp::variant v;
    memepp::string s("set via set<T>");
    REQUIRE(v.set(s) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::string));
    REQUIRE(v.get_or<memepp::string>() == "set via set<T>");
}

TEST_CASE("memepp::variant set buffer", "[variant]")
{
    const uint8_t raw[] = { 0xEE, 0xFF };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(2));
    memepp::variant v;
    REQUIRE(v.set(buf) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::buffer));
    auto val = v.get_or<memepp::buffer>();
    REQUIRE(val.size() == 2);
}

TEST_CASE("memepp::variant set variable_buffer", "[variant]")
{
    memepp::variable_buffer vb;
    vb.push_back(3);
    vb.push_back(4);
    memepp::variant v;
    REQUIRE(v.set(vb) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::varbuf));
    auto val = v.get_or<memepp::variable_buffer>();
    REQUIRE(val.size() == 2);
}

TEST_CASE("memepp::variant set rune", "[variant]")
{
    memepp::variant v;
    memepp::rune r('R');
    REQUIRE(v.set(r) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::rune));
    REQUIRE(v.get_or<memepp::rune>() == 'R');
}

// ---------------------------------------------------------------------------
// get_or — missing types (indirectly via try_get)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant get_or rune", "[variant]")
{
    memepp::rune r('G');
    memepp::variant v(r);
    REQUIRE(v.get_or<memepp::rune>() == 'G');
}

TEST_CASE("memepp::variant get_or uint64_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(555ULL));
    REQUIRE(v.get_or<uint64_t>() == 555ULL);
}

TEST_CASE("memepp::variant get_or string_view", "[variant]")
{
    memepp::string s("sv get_or");
    memepp::variant v(s);
    auto sv = v.get_or<memepp::string_view>();
    REQUIRE(sv == "sv get_or");
}

TEST_CASE("memepp::variant get_or buffer_view", "[variant]")
{
    const uint8_t raw[] = { 1, 2 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(2));
    memepp::variant v(buf);
    auto bv = v.get_or<memepp::buffer_view>();
    REQUIRE(bv.size() == 2);
}

// ---------------------------------------------------------------------------
// convert_or — missing types
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant convert_or mmint_t", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(77LL));
    REQUIRE(v.convert_or<mmint_t>() == static_cast<mmint_t>(77));
}

TEST_CASE("memepp::variant convert_or size_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(200ULL));
    REQUIRE(v.convert_or<size_t>() == static_cast<size_t>(200));
}

TEST_CASE("memepp::variant convert_or double", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    REQUIRE(v.convert_or<double>() == Approx(42.0));
}

TEST_CASE("memepp::variant convert_or with default on failure", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.convert_or<mmint_t>(static_cast<mmint_t>(-1)) == static_cast<mmint_t>(-1));
}

// ---------------------------------------------------------------------------
// DLL import / export (compile-time + basic roundtrip)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant import_from_dll const ref", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(123LL));
    auto imported = memepp::import_from_dll<memepp::variant>(
        src.native_handle(), MMVAR__OBJ_SIZE);
    REQUIRE(imported.is_type(memepp::meta::typid::int64));
    REQUIRE(imported.get_or<int64_t>() == 123LL);
}

TEST_CASE("memepp::variant export_into_dll const ref", "[variant]")
{
    memepp::variant src(static_cast<double>(2.5));
    auto exported = memepp::export_into_dll<mmvarstk_t>(
        src, MMVAR__OBJ_SIZE);
    memepp::variant v(std::move(exported));
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
    REQUIRE(v.get_or<double>() == Approx(2.5));
}

// ---------------------------------------------------------------------------
// is_type — verify all type IDs
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant is_type all known types", "[variant]")
{
    REQUIRE(memepp::variant(static_cast<mmbyte_t>(0)).is_type(memepp::meta::typid::byte));
    REQUIRE(memepp::variant('X').is_type(memepp::meta::typid::char_t));
    REQUIRE(memepp::variant(wchar_t(L'X')).is_type(memepp::meta::typid::wchar));
    REQUIRE(memepp::variant(static_cast<int64_t>(0)).is_type(memepp::meta::typid::int64));
    REQUIRE(memepp::variant(static_cast<uint64_t>(0)).is_type(memepp::meta::typid::uint64));
    REQUIRE(memepp::variant(0.0).is_type(memepp::meta::typid::double_t));
    REQUIRE(memepp::variant(memepp::string("s")).is_type(memepp::meta::typid::string));
    REQUIRE(memepp::variant(memepp::rune('A')).is_type(memepp::meta::typid::rune));

    memepp::variable_buffer vb;
    REQUIRE(memepp::variant(vb).is_type(memepp::meta::typid::varbuf));

    const uint8_t raw[] = { 0 };
    memepp::buffer buf(raw, static_cast<memepp::buffer::size_type>(1));
    REQUIRE(memepp::variant(buf).is_type(memepp::meta::typid::buffer));
}

// ---------------------------------------------------------------------------
// boundary / edge cases
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant set overwrites previous type", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(1LL));
    REQUIRE(v.set(static_cast<double>(2.0)) == 0);
    REQUIRE(v.is_type(memepp::meta::typid::double_t));
    REQUIRE(v.get_or<double>() == Approx(2.0));
}

TEST_CASE("memepp::variant operator= overwrites previous type", "[variant]")
{
    memepp::variant v('A');
    v = static_cast<int64_t>(999LL);
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    REQUIRE(v.get_or<int64_t>() == 999LL);
}

// ---------------------------------------------------------------------------
// platform-conditional try_convert specializations
// ---------------------------------------------------------------------------

#if INTPTR_MAX != INT32_MAX
TEST_CASE("memepp::variant try_convert to int32_t", "[variant]")
{
    memepp::variant v(static_cast<int64_t>(42LL));
    int32_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<int32_t>(42));
}

TEST_CASE("memepp::variant try_convert to uint32_t", "[variant]")
{
    memepp::variant v(static_cast<uint64_t>(100ULL));
    uint32_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<uint32_t>(100));
}
#endif

// ---------------------------------------------------------------------------
// platform-conditional set<T> specializations
// ---------------------------------------------------------------------------

#if INTPTR_MAX != INT32_MAX
TEST_CASE("memepp::variant set int32_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<int32_t>(-500)) == 0);
    mmint_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<mmint_t>(-500));
}

TEST_CASE("memepp::variant set uint32_t", "[variant]")
{
    memepp::variant v;
    REQUIRE(v.set(static_cast<uint32_t>(500)) == 0);
    size_t out = 0;
    REQUIRE(v.try_convert(out) == 0);
    REQUIRE(out == static_cast<size_t>(500));
}
#endif

// ---------------------------------------------------------------------------
// DLL rvalue overloads (export_into_dll with move, import_from_dll with rvalue)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::variant import_from_dll rvalue ref", "[variant]")
{
    memepp::variant::native_handle_type nh;
    MemeVariantStack_initByInt64(&nh, MMVAR__OBJ_SIZE, 777LL);
    auto imported = memepp::import_from_dll<memepp::variant>(
        std::move(nh), MMVAR__OBJ_SIZE);
    REQUIRE(imported.is_type(memepp::meta::typid::int64));
    REQUIRE(imported.get_or<int64_t>() == 777LL);
}

TEST_CASE("memepp::variant export_into_dll rvalue ref", "[variant]")
{
    memepp::variant src(static_cast<int64_t>(88LL));
    auto exported = memepp::export_into_dll<mmvarstk_t>(
        std::move(src), MMVAR__OBJ_SIZE);
    memepp::variant v(std::move(exported));
    REQUIRE(v.is_type(memepp::meta::typid::int64));
    REQUIRE(v.get_or<int64_t>() == 88LL);
}
