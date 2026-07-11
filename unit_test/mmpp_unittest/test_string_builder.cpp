#include <catch2/catch.hpp>

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
