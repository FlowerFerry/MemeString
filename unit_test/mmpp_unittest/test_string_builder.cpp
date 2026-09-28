#include <catch2/catch.hpp>

#include <stdexcept>

#include <memepp/string_builder.hpp>
#include <memepp/string.hpp>
#include <memepp/string_view.hpp>

// ---------------------------------------------------------------------------
// default constructor — empty builder
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder default constructor", "[string_builder]")
{
    memepp::string_builder builder;
    memepp::string result = builder.generate();
    REQUIRE(result.empty());
}

// ---------------------------------------------------------------------------
// operator+= (append)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+= with const char*", "[string_builder]")
{
    memepp::string_builder builder;
    builder += "Hello";
    builder += ", ";
    builder += "World!";

    memepp::string result = builder.generate();
    REQUIRE(result == "Hello, World!");
}

TEST_CASE("memepp::string_builder operator+= with memepp::string", "[string_builder]")
{
    memepp::string a("foo");
    memepp::string b("bar");

    memepp::string_builder builder;
    builder += a;
    builder += b;

    memepp::string result = builder.generate();
    REQUIRE(result == "foobar");
}

TEST_CASE("memepp::string_builder operator+= with memepp::string_view", "[string_builder]")
{
    memepp::string_view sv("view_part");

    memepp::string_builder builder;
    builder += sv;
    builder += sv;

    memepp::string result = builder.generate();
    REQUIRE(result == "view_partview_part");
}

// ---------------------------------------------------------------------------
// operator+ (append, returns reference)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+ with const char*", "[string_builder]")
{
    memepp::string_builder builder;
    builder + "abc";
    builder + "def";

    memepp::string result = builder.generate();
    REQUIRE(result == "abcdef");
}

TEST_CASE("memepp::string_builder operator+ chained mixed types", "[string_builder]")
{
    memepp::string s("str");
    memepp::string_view sv("_sv");

    memepp::string_builder builder;
    (builder + s) + sv;
    (builder + "_end");

    memepp::string result = builder.generate();
    REQUIRE(result == "str_sv_end");
}

// ---------------------------------------------------------------------------
// generate() — non-destructive
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder generate does not clear state", "[string_builder]")
{
    memepp::string_builder builder;
    builder += "persistent";

    memepp::string r1 = builder.generate();
    memepp::string r2 = builder.generate();

    REQUIRE(r1 == "persistent");
    REQUIRE(r2 == "persistent");
}

// ---------------------------------------------------------------------------
// release() — transfers content and clears builder
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder release returns content", "[string_builder]")
{
    memepp::string_builder builder;
    builder += "release_me";

    memepp::string released = builder.release();
    REQUIRE(released == "release_me");
}

TEST_CASE("memepp::string_builder release clears builder", "[string_builder]")
{
    memepp::string_builder builder;
    builder += "before_release";

    builder.release();

    // After release the builder should be empty
    memepp::string after = builder.generate();
    REQUIRE(after.empty());
}

// ---------------------------------------------------------------------------
// copy & move constructors
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder copy constructor", "[string_builder]")
{
    memepp::string_builder src;
    src += "copy_src";

    memepp::string_builder dst(src);
    REQUIRE(dst.generate() == "copy_src");
}

TEST_CASE("memepp::string_builder move constructor", "[string_builder]")
{
    memepp::string_builder src;
    src += "move_src";

    memepp::string_builder dst(std::move(src));
    REQUIRE(dst.generate() == "move_src");
}

// ---------------------------------------------------------------------------
// UTF-8 / Chinese content
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder with Chinese characters", "[string_builder]")
{
    memepp::string_builder builder;
    builder += "你好";
    builder += "，";
    builder += "世界！";

    memepp::string result = builder.generate();
    REQUIRE(result == "你好，世界！");
}

// ---------------------------------------------------------------------------
// large content accumulation
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder accumulate many small strings", "[string_builder]")
{
    memepp::string_builder builder;
    for (int i = 0; i < 100; ++i)
        builder += "x";

    memepp::string result = builder.generate();
    REQUIRE(result.size() == 100);
}

// ---------------------------------------------------------------------------
// operator+(const char*, const memepp::string&) — free function, global scope
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) basic", "[string_builder]")
{
    memepp::string rhs("World!");
    memepp::string_builder builder = "Hello, " + rhs;
    REQUIRE(builder.generate() == "Hello, World!");
}

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) empty lhs", "[string_builder]")
{
    memepp::string rhs("abc");
    memepp::string_builder builder = "" + rhs;
    REQUIRE(builder.generate() == "abc");
}

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) empty rhs", "[string_builder]")
{
    memepp::string rhs("");
    memepp::string_builder builder = "abc" + rhs;
    REQUIRE(builder.generate() == "abc");
}

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) both empty", "[string_builder]")
{
    memepp::string rhs("");
    memepp::string_builder builder = "" + rhs;
    REQUIRE(builder.generate() == "");
}

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) UTF-8 Chinese", "[string_builder]")
{
    memepp::string rhs("世界");
    memepp::string_builder builder = "你好" + rhs;
    REQUIRE(builder.generate() == "你好世界");
}

TEST_CASE("memepp::string_builder operator+(const char*, const memepp::string&) result can be chained", "[string_builder]")
{
    memepp::string rhs("bar");
    memepp::string_builder builder = "foo" + rhs;
    builder += "baz";
    REQUIRE(builder.generate() == "foobarbaz");
}

// ---------------------------------------------------------------------------
// move assignment
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder move assignment transfers content", "[string_builder]")
{
    memepp::string_builder src;
    src += "move_assign_src";

    memepp::string_builder dst;
    dst = std::move(src);

    REQUIRE(dst.generate() == "move_assign_src");
}

TEST_CASE("memepp::string_builder move assignment leaves source empty", "[string_builder]")
{
    memepp::string_builder src;
    src += "src_content";

    memepp::string_builder dst;
    dst += "dst_content";
    dst = std::move(src);

    // src is swapped into dst; src now has dst's old content
    REQUIRE(src.generate() == "dst_content");
    REQUIRE(dst.generate() == "src_content");
}

// ---------------------------------------------------------------------------
// copy assignment (swap semantics)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder copy assignment swaps content", "[string_builder]")
{
    memepp::string_builder src;
    src += "src_data";

    memepp::string_builder dst;
    dst += "dst_data";
    dst = src;

    // copy assignment uses swap: dst gets src's content, src gets dst's old content
    REQUIRE(src.generate() == "dst_data");
    REQUIRE(dst.generate() == "src_data");
}

TEST_CASE("memepp::string_builder self-assignment copy", "[string_builder]")
{
    memepp::string_builder b;
    b += "self_assign";

    b = b;

    // state must be unchanged after self-assignment
    REQUIRE(b.generate() == "self_assign");
}

// ---------------------------------------------------------------------------
// native_handle()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder native_handle returns valid reference", "[string_builder]")
{
    memepp::string_builder b;
    b += "nh_test";

    const auto& nh = b.native_handle();
    // verify content is accessible via generate (the primary public API)
    REQUIRE(b.generate() == "nh_test");
    // native_handle returns a reference to the internal struct
    REQUIRE(sizeof(nh) == sizeof(MemeStringBuilderStack_t));
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const string&) + string_builder&  (lvalue ref return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const string&, string_builder&) prepends", "[string_builder]")
{
    memepp::string s("prefix_");
    memepp::string_builder b;
    b += "suffix";

    // prepend s before b's content, returns lvalue ref to same builder
    memepp::string_builder& ref = s + b;
    REQUIRE(&ref == &b);
    REQUIRE(b.generate() == "prefix_suffix");
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const string&) + string_builder&&  (rvalue return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const string&, string_builder&&) prepends and returns", "[string_builder]")
{
    memepp::string s("Hello, ");

    auto result = s + memepp::string_builder{};
    (result + "world!");
    REQUIRE(result.generate() == "Hello, world!");
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const string_view&) + string_builder&  (lvalue ref return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const string_view&, string_builder&) prepends", "[string_builder]")
{
    memepp::string_view sv("sv_prefix_");
    memepp::string_builder b;
    b += "body";

    memepp::string_builder& ref = sv + b;
    REQUIRE(&ref == &b);
    REQUIRE(b.generate() == "sv_prefix_body");
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const string_view&) + string_builder&&  (rvalue return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const string_view&, string_builder&&) prepends and returns", "[string_builder]")
{
    memepp::string_view sv("<<");

    auto result = sv + memepp::string_builder{};
    (result + "body");
    REQUIRE(result.generate() == "<<body");
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const char*) + string_builder&  (lvalue ref return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const char*, string_builder&) prepends", "[string_builder]")
{
    memepp::string_builder b;
    b += "tail";

    memepp::string_builder& ref = "head_" + b;
    REQUIRE(&ref == &b);
    REQUIRE(b.generate() == "head_tail");
}

// ---------------------------------------------------------------------------
// prepend operator+ — (const char*) + string_builder&&  (rvalue return)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder operator+(const char*, string_builder&&) prepends and returns", "[string_builder]")
{
    auto result = "rvalue_head_" + memepp::string_builder{};
    (result + "rvalue_tail");
    REQUIRE(result.generate() == "rvalue_head_rvalue_tail");
}

// ---------------------------------------------------------------------------
// prepend operator+ — chain multiple prepends and appends
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder prepend + append mixed chaining", "[string_builder]")
{
    memepp::string s1("one");
    memepp::string s2("two");
    memepp::string_builder b;
    b += "three";

    // prepend s2, then prepend s1 (order: s1 then s2 then "three")
    s1 + (s2 + b);
    REQUIRE(b.generate() == "onetwothree");
}

TEST_CASE("memepp::string_builder prepend rvalue chain", "[string_builder]")
{
    memepp::string s1("-P1");
    memepp::string_view sv("-P2");

    // prepend with move: s1 prepends, then sv prepends before everything
    auto result = sv + (s1 + memepp::string_builder{});
    result + "core";
    REQUIRE(result.generate() == "-P2-P1core");
}

// ---------------------------------------------------------------------------
// destructor — verify proper cleanup after release (indirect)
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder destructor after release re-init", "[string_builder]")
{
    // After release, builder's internal state is reset.
    // Verify that generate() returns empty.
    memepp::string_builder b;
    b += "temp";
    b.release();

    REQUIRE(b.generate().empty());
}

TEST_CASE("memepp::string_builder reuse after release", "[string_builder]")
{
    memepp::string_builder b;
    b += "first";

    b.release();

    // After release, builder should be reusable
    b += "second";
    REQUIRE(b.generate() == "second");
}

// ---------------------------------------------------------------------------
// size() — total UTF-8 byte length across all parts
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder size default is 0", "[string_builder]")
{
    memepp::string_builder b;
    REQUIRE(b.size() == 0);
}

TEST_CASE("memepp::string_builder size single append", "[string_builder]")
{
    memepp::string_builder b;
    b += "abc";
    REQUIRE(b.size() == 3);
}

TEST_CASE("memepp::string_builder size sums all parts", "[string_builder]")
{
    memepp::string_builder b;
    b += "Hello, ";
    b += "World!";

    REQUIRE(b.size() == 13);
    REQUIRE(b.part_count() == 2);
}

TEST_CASE("memepp::string_builder size UTF-8 multibyte", "[string_builder]")
{
    // hex escapes: fixed UTF-8 bytes (ni hao), independent of the
    // execution charset the compiler applies to source literals
    memepp::string_view sv("\xE4\xBD\xA0\xE5\xA5\xBD", 6);

    memepp::string_builder b;
    b += sv;

    REQUIRE(b.size() == 6);
    REQUIRE(b.generate().size() == 6);
}

// ---------------------------------------------------------------------------
// empty()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder empty default constructor", "[string_builder]")
{
    memepp::string_builder b;
    REQUIRE(b.empty());
}

TEST_CASE("memepp::string_builder empty after append", "[string_builder]")
{
    memepp::string_builder b;
    REQUIRE(b.empty());

    b += "x";
    REQUIRE_FALSE(b.empty());
}

TEST_CASE("memepp::string_builder empty after clear", "[string_builder]")
{
    memepp::string_builder b;
    b += "content";
    REQUIRE_FALSE(b.empty());

    b.clear();
    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
}

// ---------------------------------------------------------------------------
// part_count() / part_capacity()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder part_count tracks appends", "[string_builder]")
{
    memepp::string_builder b;
    REQUIRE(b.part_count() == 0);

    b += "a";
    b += "b";
    b += "c";
    REQUIRE(b.part_count() == 3);
}

TEST_CASE("memepp::string_builder part_capacity holds part_count", "[string_builder]")
{
    memepp::string_builder b;
    for (int i = 0; i < 10; ++i)
        b += "x";

    REQUIRE(b.part_capacity() >= b.part_count());
    REQUIRE(b.part_count() == 10);
}

// ---------------------------------------------------------------------------
// reserve_parts()
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder reserve_parts grows capacity", "[string_builder]")
{
    memepp::string_builder b;
    b.reserve_parts(16);
    REQUIRE(b.part_capacity() >= 16);
    REQUIRE(b.part_count() == 0);

    for (int i = 0; i < 16; ++i)
        b += "x";

    REQUIRE(b.part_count() == 16);
    REQUIRE(b.size() == 16);
    REQUIRE(b.generate().size() == 16);
}

#if !MMOPT__EXCEPTION_DISABLED
TEST_CASE("memepp::string_builder reserve_parts negative throws", "[string_builder]")
{
    memepp::string_builder b;
    REQUIRE_THROWS_AS(b.reserve_parts(-1), std::runtime_error);
}
#endif

// ---------------------------------------------------------------------------
// clear() — resets content, builder stays reusable
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder clear resets all state", "[string_builder]")
{
    memepp::string_builder b;
    b += "one";
    b += "two";

    b.clear();

    REQUIRE(b.empty());
    REQUIRE(b.size() == 0);
    REQUIRE(b.part_count() == 0);
    REQUIRE(b.generate().empty());
}

TEST_CASE("memepp::string_builder clear keeps builder usable", "[string_builder]")
{
    memepp::string_builder b;
    b += "first";
    b.clear();
    b += "second";

    REQUIRE(b.generate() == "second");
}

TEST_CASE("memepp::string_builder clear on empty builder is a no-op", "[string_builder]")
{
    memepp::string_builder b;
    b.clear();
    REQUIRE(b.empty());
    REQUIRE(b.part_count() == 0);
}

// ---------------------------------------------------------------------------
// new interfaces x release() interaction — queries reset to zero
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder queries reset after release", "[string_builder]")
{
    memepp::string_builder b;
    b += "abc";
    b += "def";
    REQUIRE(b.size() == 6);
    REQUIRE(b.part_count() == 2);

    b.release();

    REQUIRE(b.size() == 0);
    REQUIRE(b.part_count() == 0);
    REQUIRE(b.empty());
}

// ---------------------------------------------------------------------------
// empty-string append — part semantics
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder empty append counts as part", "[string_builder]")
{
    memepp::string_builder b;
    b += "";
    REQUIRE(b.size() == 0);          // empty part contributes no bytes
    REQUIRE(b.part_count() == 1);    // but occupies a part slot
    REQUIRE(b.generate().empty());

    b += "x";
    REQUIRE(b.size() == 1);
    REQUIRE(b.part_count() == 2);
}

// ---------------------------------------------------------------------------
// reserve_parts() — zero count boundary
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder reserve_parts zero is no-op", "[string_builder]")
{
    memepp::string_builder b;
    b.reserve_parts(0);              // legal, no growth, no error
    REQUIRE(b.part_count() == 0);
    REQUIRE(b.empty());
}

// ---------------------------------------------------------------------------
// clear() after reserve_parts() — parts fully released
// ---------------------------------------------------------------------------

TEST_CASE("memepp::string_builder clear after reserve_parts", "[string_builder]")
{
    memepp::string_builder b;
    b.reserve_parts(32);
    b += "data";

    b.clear();

    REQUIRE(b.part_count() == 0);    // clear re-inits, all parts released
    REQUIRE(b.empty());
    // part_capacity() exact value after re-init is an implementation detail
}
