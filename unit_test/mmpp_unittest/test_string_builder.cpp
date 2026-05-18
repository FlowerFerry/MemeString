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
