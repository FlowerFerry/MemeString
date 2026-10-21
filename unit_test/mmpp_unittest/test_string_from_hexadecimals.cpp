
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>

#include <vector>


TEST_CASE("memepp::string from_hexadecimals", "[string]")
{
    REQUIRE(memepp::from_hexadecimals(NULL,  0) == "");
    REQUIRE(memepp::from_hexadecimals(NULL,  1) == "");
    // Exercise the C layer's defensive _len <= 0 path:
    // size_t(-1) wraps back to MemeInteger_t(-1) inside the C API.
    REQUIRE(memepp::from_hexadecimals(NULL, static_cast<size_t>(-1)) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL,  0) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL,  1) == "");
    REQUIRE(memepp::from_hexadecimals("", NULL, static_cast<size_t>(-1)) == "");

    uint8_t buf01_01[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    auto str01_01 = memepp::from_hexadecimals(buf01_01, sizeof(buf01_01));
    REQUIRE(str01_01.size() == sizeof(buf01_01) * 2);
    REQUIRE(str01_01 == "000102030405060708090A0B0C0D0E0F");

    auto str01_02 = memepp::from_hexadecimals(" ", buf01_01, sizeof(buf01_01));
    REQUIRE(str01_02.size() == sizeof(buf01_01) * 3 - 1);
    REQUIRE(str01_02 == "00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");

    // Exercise the C layer's defensive _len <= 0 path via size_t(-1) wrap.
    auto str01_03 = memepp::from_hexadecimals(buf01_01, static_cast<size_t>(-1));
    REQUIRE(str01_03 == "");

    auto str01_04 = memepp::from_hexadecimals(" ", buf01_01, static_cast<size_t>(-1));
    REQUIRE(str01_04 == "");

    // High-byte values (0xF0, 0xFF) exercise hex digits 'F' in both nibble positions.
    {
        uint8_t high[] = { 0xFF, 0xF0 };
        auto s = memepp::from_hexadecimals(high, sizeof(high));
        REQUIRE(s == "FFF0");
    }

    // Multi-character interval.
    {
        uint8_t buf[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
        auto s = memepp::from_hexadecimals("::", buf, sizeof(buf));
        REQUIRE(s == "00::01::02::03::04");
        REQUIRE(s.size() == 5 * 2 + 4 * 2);  // 5 pairs + 4 separators of 2 chars
    }

    // Interval with embedded NUL — the NUL byte is copied verbatim into the output.
    {
        uint8_t buf[] = { 0xAA, 0xBB, 0xCC };
        char interval[] = { '-', '\0', '+' };
        auto s = memepp::from_hexadecimals(
            memepp::string_view(interval, 3), buf, sizeof(buf));
        REQUIRE(s.size() == 3 * 2 + 2 * 3);  // 6 hex chars + 2 separators of 3 bytes
        // Bytes: "AA" '-' '\0' '+' "BB" '-' '\0' '+' "CC"
        //        0   1   2   3   4   5   6   7   8   9  10  11
        REQUIRE(s[2] == '-');
        REQUIRE(s[3] == '\0');
        REQUIRE(s[4] == '+');
        REQUIRE(s[8] == '\0');
    }

    // NULL interval pointer with positive interval length — interval is ignored.
    // Not reachable from the C++ wrapper (no 4-arg overload); test via C API.
    {
        uint8_t buf[] = { 0xFF, 0xFE };
        mmstrstk_t stk;
        int rc = mmstrstk_init_by_hex(&stk, NULL, 5, buf, sizeof(buf));
        REQUIRE(rc == 0);
        memepp::string s(std::move(stk));
        REQUIRE(s == "FFFE");
    }

    // Medium-range result (output exceeds small buffer limit).
    // 50 input bytes with 1-char interval -> 50*2 + 49*1 = 149 output bytes.
    {
        std::vector<uint8_t> buf(50, 0xAB);
        auto s = memepp::from_hexadecimals("-", buf.data(), buf.size());
        REQUIRE(s.size() == 149);
        REQUIRE(s.substr(0, 5) == "AB-AB");
        REQUIRE(s.substr(147) == "AB");  // last pair at positions 147-148
    }

    // Large input -> 1000 output bytes (well beyond small limit).
    {
        std::vector<uint8_t> buf(500, 0xCD);
        auto s = memepp::from_hexadecimals(buf.data(), buf.size());
        REQUIRE(s.size() == 1000);
        REQUIRE(s.substr(0, 4) == "CDCD");
        REQUIRE(s.substr(996) == "CDCD");
    }
}

TEST_CASE("memepp::string from_hexadecimals - user storage input", "[string]")
{
    // from_hexadecimals() itself always yields small/medium/large storage.
    // Guard that a user-storage source in the same scope does not disturb it.
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'A');
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    uint8_t buf[] = { 0xAB, 0xCD, 0xEF };
    memepp::string result = memepp::from_hexadecimals("-", buf, sizeof(buf));
    REQUIRE(result == "AB-CD-EF");
}

TEST_CASE("memepp::string from_hexadecimals - view storage input", "[string]")
{
    // from_hexadecimals() itself always yields small/medium/large storage.
    // Guard that a view-storage source in the same scope does not disturb it.
    const char* src = "ABC";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    uint8_t buf[] = { 0x12, 0x34 };
    memepp::string result = memepp::from_hexadecimals("", buf, sizeof(buf));
    REQUIRE(result == "1234");

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
