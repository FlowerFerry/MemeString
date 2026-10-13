
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/errc.hpp>
#include <memepp/convert/std/c_str.hpp>
#include <meme/unsafe/string_view.h>

#include <cstdlib>
#include <cstring>

TEST_CASE("memepp::string reset", "[string]")
{
    memepp::string str01 = "0123456789";
    str01.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(str01.size() == 0);
    REQUIRE(str01.storage_type() == memepp::str_strg_t::small);
    REQUIRE(str01.empty() == true);

    memepp::string str02 = "01234567890123456789012345678901234567890123456789012345678901234567890123456789";
    // Verify medium storage before reset
    REQUIRE(str02.storage_type() == memepp::str_strg_t::medium);
    str02.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(str02.size() == 0);
    REQUIRE(str02.storage_type() == memepp::str_strg_t::small);
    REQUIRE(str02.empty() == true);

    memepp::string str03 = 
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    // Verify initial large storage
    REQUIRE(str03.size() == 500);
    REQUIRE(str03.storage_type() == memepp::str_strg_t::large);
    
    // Reset and verify
    str03.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(str03.size() == 0);
    REQUIRE(str03.storage_type() == memepp::str_strg_t::small);
    REQUIRE(str03.empty() == true);
}

TEST_CASE("memepp::string reset - user storage", "[string]")
{
    // Use convert::takeover with a long buffer to force user storage
    char* p = (char*)malloc(1024);
    REQUIRE(p != nullptr);
    memset(p, 'X', 1023);
    p[1023] = '\0';

    memepp::string s = memepp::convert::takeover(p, -1, free);
    REQUIRE(s.size() == 1023);
    REQUIRE(s.storage_type() == memepp::string_storage_t::user);

    s.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(s.size() == 0);
    REQUIRE(s.empty() == true);
    REQUIRE(s.storage_type() == memepp::str_strg_t::small);
}

TEST_CASE("memepp::string reset - view storage via C API", "[string]")
{
    // C++ wrapper cannot produce view-type string (constructor deep-copies views).
    // Use C API directly to create a view, then reset it.
    const char* src = "hello_view_reset_test";
    mmstrstk_t raw;
    int rc = MemeStringViewUnsafeStack_init(&raw, sizeof(raw),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType(memepp::to_pointer(raw)) == MemeString_UnsafeStorageType_view);

    rc = MemeStringStack_reset(&raw, sizeof(raw));
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType(memepp::to_pointer(raw)) == MemeString_StorageType_small);
    REQUIRE(MemeString_byteSize(memepp::to_pointer(raw)) == 0);

    MemeStringStack_unInit(&raw, sizeof(raw));
}

TEST_CASE("memepp::string reset - empty string", "[string]")
{
    memepp::string s; // default-constructed, empty small
    REQUIRE(s.empty() == true);
    REQUIRE(s.storage_type() == memepp::str_strg_t::small);

    s.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(s.empty() == true);
    REQUIRE(s.size() == 0);
    REQUIRE(s.storage_type() == memepp::str_strg_t::small);
}

TEST_CASE("memepp::string reset - shared large copy", "[string]")
{
    // Build a large string (>500 bytes)
    memepp::string original = 
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
        "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    REQUIRE(original.size() == 500);
    REQUIRE(original.storage_type() == memepp::str_strg_t::large);

    // Copy shares the refcounted storage
    memepp::string copy(original);
    REQUIRE(copy.size() == 500);
    REQUIRE(copy.storage_type() == memepp::str_strg_t::large);
    REQUIRE(copy == original);

    // Reset original — copy must remain intact (refcount semantics)
    original.reset();
    REQUIRE(memepp::get_errc() == 0);
    REQUIRE(original.size() == 0);
    REQUIRE(original.empty() == true);
    REQUIRE(original.storage_type() == memepp::str_strg_t::small);

    // Copy must still hold the full 500-byte content
    REQUIRE(copy.size() == 500);
    REQUIRE(copy.storage_type() == memepp::str_strg_t::large);
    REQUIRE(copy.empty() == false);
    REQUIRE(copy.data()[0] == '0');
    REQUIRE(copy.data()[499] == '9');
}
