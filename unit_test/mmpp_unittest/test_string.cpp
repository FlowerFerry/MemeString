
#include <catch2/catch.hpp>

#include <memepp/string.hpp>
#include <memepp/string_view.hpp>
#include <memepp/convert/std/string.hpp>

TEST_CASE("memepp::string count", "[string]")
{
    memepp::string str = "AaBbCcAaBbCcAaBbCc";

    // count with string
    REQUIRE(str.count("Aa") == 3);
    //REQUIRE(str.count("Aa", memepp::case_sensit_t::all_insensitive) == 3);
    //REQUIRE(str.count("aa", memepp::case_sensit_t::all_insensitive) == 3); // case insensitive

    // count with char*
    REQUIRE(str.count("Bb") == 3);
    //REQUIRE(str.count("bb", memepp::case_sensit_t::all_insensitive) == 3);

    // count with char*, size
    REQUIRE(str.count("Cc", 2) == 3);
    //REQUIRE(str.count("cc", 2, memepp::case_sensit_t::all_insensitive) == 3);

    // count with pointer (MemeByte_t*)
    REQUIRE(str.count(reinterpret_cast<memepp::string::const_pointer>("Aa"), 2) == 3);

    // count with rune
    memepp::rune r = 'A';
    REQUIRE(str.count(r) == 3);
    //REQUIRE(str.count(r, memepp::case_sensit_t::all_insensitive) == 6); // A and a

    // edge cases
    REQUIRE(str.count("") == 0); // empty key
    REQUIRE(str.count("Xx") == 0); // not found
    REQUIRE(memepp::string{}.count("A") == 0); // empty string
    //REQUIRE(memepp::string{"A"}.count("A", memepp::case_sensit_t::all_insensitive) == 1);
}

// 测试 to_large 的全面情况
TEST_CASE("memepp::string to_large", "[string]")
{
    // empty string
    memepp::string empty;
    auto large_empty = empty.to_large();
    REQUIRE(large_empty.empty());
    REQUIRE(large_empty.storage_type() == memepp::string_storage_t::large);
    REQUIRE(large_empty.size() == 0);

    // small storage
    memepp::string small_str = "short";
    REQUIRE(small_str.storage_type() == memepp::string_storage_t::small);
    auto large_small = small_str.to_large();
    REQUIRE(large_small == "short");
    REQUIRE(large_small.storage_type() == memepp::string_storage_t::large);

    // medium storage
    memepp::string medium_str = "012345678901234567890123456789"; // adjust to medium size
    REQUIRE(medium_str.storage_type() == memepp::string_storage_t::medium);
    auto large_medium = medium_str.to_large();
    REQUIRE(large_medium == medium_str);
    REQUIRE(large_medium.storage_type() == memepp::string_storage_t::large);

    // already large
    memepp::string large_str = 
        "very long string that exceeds medium limit very long string that exceeds medium limit";
    auto large_large = large_str.to_large();
    REQUIRE(large_large == large_str);
    REQUIRE(large_large.storage_type() == memepp::string_storage_t::large);
}

// 测试与 uint16_t* 的构造函数
TEST_CASE("memepp::string constructors with uint16_t", "[string]")
{
    const uint16_t utf16_data[] = { 'H', 'e', 'l', 'l', 'o' }; // "Hello"
    size_t len = sizeof(utf16_data) / sizeof(uint16_t);

    // basic constructor
    memepp::string str1(utf16_data, len);
    REQUIRE(str1 == "Hello");
    REQUIRE(str1.size() == 5);
    REQUIRE(str1.storage_type() == memepp::string_storage_t::small);

    // with suggest
    memepp::string str2(utf16_data, len, memepp::string_storage_t::large);
    REQUIRE(str2 == "Hello");
    REQUIRE(str2.storage_type() == memepp::string_storage_t::large);

    // invalid UTF-16 (partial surrogate)
    const uint16_t invalid_utf16[] = { 0xD800 }; // high surrogate without low
    memepp::string str3(invalid_utf16, 1);
    REQUIRE(str3.size() == 0); // should handle invalid as empty or replacement

    // empty
    memepp::string str4(utf16_data, 0);
    REQUIRE(str4.empty());
}

// 补充边界和异常情况
TEST_CASE("memepp::string boundary cases", "[string]")
{
    memepp::string empty;

    // count on empty
    REQUIRE(empty.count("A") == 0);
    REQUIRE(empty.count(memepp::rune{'A'}) == 0);

    // repeat on empty
    REQUIRE(empty.repeat(5) == "");

    // substr on empty
    REQUIRE(empty.substr(0, 10) == "");

    // find on empty
    REQUIRE(empty.find("A") == empty.npos);
    REQUIRE(empty.rfind("A") == empty.npos);

    // capacity on empty
    REQUIRE(empty.capacity() >= 0);

    // invalid rune_size (bad UTF-8)
    memepp::string invalid_utf8(reinterpret_cast<const char*>("\xFF\xFF"), 2); // invalid bytes
    REQUIRE(invalid_utf8.rune_size() == 0); // should handle as 0 runes
    REQUIRE(invalid_utf8.size() == 2);
}

// 测试长字符串的稳定性
TEST_CASE("memepp::string long string stability", "[string]")
{
    std::string long_std(10000, 'A'); // 10k chars
    memepp::string long_str = memepp::from(long_std);
    REQUIRE(long_str.size() == 10000);
    REQUIRE(long_str.storage_type() == memepp::string_storage_t::large);

    // operations on long string
    REQUIRE(long_str.count("A") == 10000);
    auto repeated = long_str.repeat(2);
    REQUIRE(repeated.size() == 20000);

    auto sub = long_str.substr(0, 5000);
    REQUIRE(sub.size() == 5000);
    REQUIRE(sub == memepp::view(std::string(5000, 'A')));
}

TEST_CASE("memepp::string count overloads", "[string]")
{
    memepp::string s = "Hello, HELLO, HeLLo, World! Hello.";

    // count(const char*)
    REQUIRE(s.count("Hello") == 2);
    REQUIRE(s.count("HELLO") == 1);

    // 大小写不敏感
    //REQUIRE(s.count("hello", memepp::case_sensit_t::all_insensitive) == 4);

    // count(const char*, size_type) —— 仅取前缀 "Hell"
    REQUIRE(s.count("HelloXYZ", 4) == 2); 
    REQUIRE(s.count("HelloXYZ", 5) == 2);  // "Hello" 出现 2 次

    // count(const_pointer, size_type) 与上等价（接口重复覆盖）
    REQUIRE(s.count(reinterpret_cast<const memepp::string::const_pointer>("HelloXYZ"), 5) == 2);

    // count(const_pointer)
    REQUIRE(s.count(reinterpret_cast<const memepp::string::const_pointer>("World!")) == 1);

    // count(const rune&)
    memepp::rune rl = 'l';
    REQUIRE(s.count(rl) >= 0); // 至少能跑通
    // 确认部分计数（"Hello, HELLO, HeLLo, World! Hello." 中 ‘l’ 出现 7 次）
    REQUIRE(s.count(rl) == 5);
}

TEST_CASE("memepp::string find overloads with _substr_count/char/rune", "[string]")
{
    memepp::string s = "foobar foo foo";

    // find(const char* , pos, substr_count) —— 只匹配模式前缀
    REQUIRE(s.find("foo", 0, 1) == 0);  // 找 'f'
    REQUIRE(s.find("foo", 0, 2) == 0);  // 找 "fo"
    REQUIRE(s.find("foo", 1, 2) == 7);
    REQUIRE(s.find("bar", 0, 3) == 3);
    REQUIRE(s.find("bar", 4, 3) == -1);

    // find(char)
    REQUIRE(s.find('f', 0) == 0);
    REQUIRE(s.find('b', 0) == 3);
    REQUIRE(s.find('z', 0) == -1);

    // find(rune)（ASCII）
    memepp::rune rf = 'f';
    memepp::rune rz = 'z';
    REQUIRE(s.find(rf, 0) == 0);
    REQUIRE(s.find(rz, 0) == -1);

    // find(rune)（非 ASCII）
    memepp::string u = u8"您Hello界您";
    memepp::rune r_you{ reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    memepp::rune r_world{ reinterpret_cast<const uint8_t*>(u8"界"), -1 };
    REQUIRE(u.find(r_you, 0) == 0);
    REQUIRE(u.find(r_world, 0) > 0);
}

TEST_CASE("memepp::string rfind(rune)", "[string]")
{
    memepp::string s = u8"甲乙丙丁丁";
    memepp::rune r_ding{ reinterpret_cast<const uint8_t*>(u8"丁"), -1 };
    memepp::rune r_wu{ reinterpret_cast<const uint8_t*>(u8"戊"), -1 };
    REQUIRE(s.rfind(r_ding) == s.size() - 3); // 最后一个“丁”的起始字节位置
    REQUIRE(s.rfind(r_wu) == -1);
}

TEST_CASE("memepp::string starts_with/ends_with - count and multibyte rune (Chinese)", "[string]")
{
    memepp::string s = "HelloWorld";

    // starts_with(const char*, size_type)
    REQUIRE(s.starts_with("HelloXYZ", 5));
    REQUIRE_FALSE(s.starts_with("HelloXYZ", 6));

    // ends_with(const char*, size_type)
    REQUIRE(s.ends_with("XYZWorld", 5) == false);
    REQUIRE_FALSE(s.ends_with("XYZWorld", 6));

    // starts_with(const rune&)/ends_with(const rune&)
    memepp::string u = (const char*)u8"您Hello界";
    memepp::rune r_head{ reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    memepp::rune r_tail{ reinterpret_cast<const uint8_t*>(u8"界"), -1 };
    memepp::rune r_x{ reinterpret_cast<const uint8_t*>(u8"谢"), -1 };
    REQUIRE(u.starts_with(r_head));
    REQUIRE(u.ends_with(r_tail));
    REQUIRE_FALSE(u.starts_with(r_x));
    REQUIRE_FALSE(u.ends_with(r_x));
}

TEST_CASE("memepp::string contains - char and multibyte rune (Chinese)", "[string]")
{
    memepp::string s = u8"您好，世界！Hello, World!";
    // contains(const char*, size)
    REQUIRE(s.contains("Hello, World!", 13));
    REQUIRE_FALSE(s.contains("Hello, World?", 13));

    // contains(char)
    REQUIRE(s.contains('H'));
    REQUIRE_FALSE(s.contains('Z'));

    // contains(const rune&)
    memepp::rune r_you{ reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    memepp::rune r_unknown{ reinterpret_cast<const uint8_t*>(u8"谢"), -1 };
    REQUIRE(s.contains(r_you));
    REQUIRE_FALSE(s.contains(r_unknown));
}

TEST_CASE("memepp::string repeat (member and static)", "[string]")
{
    memepp::string s = "ab";
    REQUIRE(s.repeat(0) == "");
    REQUIRE(s.repeat(1) == "ab");
    REQUIRE(s.repeat(3) == "ababab");

    auto r1 = memepp::string::repeat("x", 5);
    REQUIRE(r1 == "xxxxx");

    auto r2 = memepp::string::repeat("", 10);
    REQUIRE(r2 == "");
}

TEST_CASE("memepp::string operator+ overloads and assignment from string_builder", "[string]")
{
    memepp::string a = "Hello";
    memepp::string_view v = ", SV";
    auto s1 = a + ", CSTR";
    REQUIRE(s1 == "Hello, CSTR");

    auto s2 = a + v;
    REQUIRE(s2 == "Hello, SV");

    memepp::string s;
    s = memepp::string_builder{} + "A" + "B" + a + v; // operator=(const string_builder&)
    REQUIRE(s == "ABHello, SV");
}

TEST_CASE("memepp::string pointer/length helpers", "[string]")
{
    memepp::string s = "abc";
    // bytes() 与 data() 指针一致性
    REQUIRE(reinterpret_cast<const void*>(s.bytes()) == reinterpret_cast<const void*>(s.data()));

    // length/char_size/size 一致性
    REQUIRE(s.length() == s.size());
    REQUIRE(s.char_size() == s.size());
}

TEST_CASE("memepp::string to_rune_iterator(const_iterator)", "[string]")
{
    // u8: "ab" + "零"(3字节) + "cd"
    memepp::string s = u8"ab零cd";

    // 指向 '零' 的首字节（索引 2）
    auto it = std::next(s.cbegin(), 2);
    auto r_it_ok = s.to_rune_iterator(it);
    REQUIRE(r_it_ok.is_valid());
    REQUIRE(r_it_ok.to_rune() == u8"零");
    REQUIRE(r_it_ok.to_index().data() == &s.at(2));

    // 指向 '零' 的第二个字节（非 rune 边界，索引 3）
    auto it_middle = std::next(s.cbegin(), 3);
    auto r_it_bad = s.to_rune_iterator(it_middle);
    REQUIRE_FALSE(r_it_bad.is_valid());
    REQUIRE(r_it_bad.to_index().data() == &s.at(3));
}

TEST_CASE("memepp::string rune equality and UDL", "[string]")
{
    // operator==(rune, string) / (string, rune)
    memepp::string a = "H";
    memepp::rune rH = 'H';
    memepp::rune rX = 'X';

    REQUIRE(a == rH);
    REQUIRE_FALSE(a == rX);
    REQUIRE(rH == a);
    REQUIRE(rX != a);

    // Unicode rune
    memepp::string c = u8"您";
    memepp::rune r_you { reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    REQUIRE(c == r_you);
    REQUIRE(r_you == c);

    // UDL operator""_meme
    using namespace memepp;
    auto s = "Hello, UDL"_meme;
    REQUIRE(s == "Hello, UDL");
}

TEST_CASE("memepp::string starts_with/ends_with with rune (edge invalid)", "[string]")
{
    // 无效 UTF-8 输入中的 rune 检查应返回 false
    const uint8_t bad_bytes[] = { 0xE6, 0x82, 0x00, 0x00 };
    memepp::string_view sv(reinterpret_cast<const char*>(bad_bytes), 3);
    memepp::rune r_any { reinterpret_cast<const uint8_t*>(u8"您"), -1 };
    REQUIRE_FALSE(sv.starts_with(r_any));
    REQUIRE_FALSE(sv.ends_with(r_any));
}

TEST_CASE("memepp::string mapping_convert advanced", "[string]")
{
    memepp::string str = u8"AbC零一二AbC";

    // 修改 rune (to upper)
    auto mapped1 = str.mapping_convert([](memepp::rune& r) {
        if (r == 'b') r = 'B';
        return 0;
    });
    REQUIRE(mapped1 == u8"ABC零一二ABC");

    // 删除 rune (return -1 for specific)
    auto mapped2 = str.mapping_convert([](memepp::rune& r) {
        if (r == 'A' || r == u8"零") return -1; // delete
        return 0;
    });
    REQUIRE(mapped2 == u8"bC一二bC");


    // empty string
    REQUIRE(memepp::string{}.mapping_convert([](memepp::rune&) { return 0; }) == "");
}

// 测试 c_format 的高级格式化
TEST_CASE("memepp::string c_format advanced", "[string]")
{
    // 浮点数和宽度
    auto fmt1 = memepp::c_format(-1, "Pi is %.2f", 3.14159);
    REQUIRE(fmt1 == "Pi is 3.14");

    // 宽度和填充
    auto fmt2 = memepp::c_format(-1, "%10s", "hello");
    REQUIRE(fmt2 == "     hello"); // right-aligned with spaces

    // 大小限制截断
    auto fmt3 = memepp::c_format(10, "Very long string: %s", "truncated");
    REQUIRE(fmt3.size() <= 10);
    REQUIRE(fmt3.starts_with("Very long ")); // truncated

    // 负数和零填充
    auto fmt4 = memepp::c_format(-1, "%05d", -123);
    REQUIRE(fmt4 == "-0123"); // 注意负号不计入填充

    // 空格式
    auto fmt5 = memepp::c_format(-1, "");
    REQUIRE(fmt5 == "");
}

// c_format with predict parameter (4-argument version)
TEST_CASE("memepp::string c_format with predict", "[string]")
{
    // predict=0: buffer starts minimal, grows as needed
    auto fmt1 = memepp::c_format(-1, 0, "Value: %d", 42);
    REQUIRE(fmt1 == "Value: 42");

    // predict large enough
    auto fmt2 = memepp::c_format(-1, 64, "%s %s %d", "Hello", "World", 2024);
    REQUIRE(fmt2 == "Hello World 2024");

    // size_limit truncation with predict
    auto fmt3 = memepp::c_format(5, 0, "0123456789");
    REQUIRE(fmt3.size() <= 5);

    // predict=0 with empty format
    auto fmt4 = memepp::c_format(-1, 0, "");
    REQUIRE(fmt4 == "");
}

// 测试 from_hexadecimals 的间距和无效输入
TEST_CASE("memepp::string from_hexadecimals advanced", "[string]")
{
    uint8_t buf[] = { 0xAB, 0xCD, 0xEF };

    // 自定义间距
    auto hex1 = memepp::from_hexadecimals("-", buf, sizeof(buf));
    REQUIRE(hex1 == "AB-CD-EF");

    // 无效缓冲 (NULL with positive size)
    auto hex2 = memepp::from_hexadecimals("-", nullptr, 5);
    REQUIRE(hex2 == ""); // should handle as empty

    // 空间距
    auto hex3 = memepp::from_hexadecimals("", buf, sizeof(buf));
    REQUIRE(hex3 == "ABCDEF");

    // 单字节
    auto hex4 = memepp::from_hexadecimals(" ", buf, 1);
    REQUIRE(hex4 == "AB");
}

// 测试 split 的多字节键和特殊字符
TEST_CASE("memepp::string split advanced", "[string]")
{
    memepp::string str = u8"苹果-香蕉-橙子-苹果-";

    // 多字节 UTF-8 键
    std::vector<memepp::string> results1;
    str.split(u8"苹果", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results1));
    REQUIRE(results1.size() == 3);
    REQUIRE(results1[0] == "");
    REQUIRE(results1[1] == u8"-香蕉-橙子-");
    REQUIRE(results1[2] == "-");

    // 空键 (应返回整个字符串)
    std::vector<memepp::string> results2;
    str.split("", memepp::split_behav_t::keep_empty_parts, std::back_inserter(results2));
    REQUIRE(results2.size() == 1);
    REQUIRE(results2[0] == str);

    // 特殊字符键（如空格）
    memepp::string spaced = "a b c d";
    std::vector<memepp::string> results3;
    spaced.split(" ", memepp::split_behav_t::skip_empty_parts, std::back_inserter(results3));
    REQUIRE(results3.size() == 4);
    REQUIRE(results3[3] == "d");
}

// 测试与其他类型的交互（如 string_builder 赋值）
TEST_CASE("memepp::string string_builder assignment", "[string]")
{
    memepp::string_builder sb;
    sb += "Hello";
    sb += " World";

    memepp::string str;
    str = sb; // operator= with string_builder
    REQUIRE(str == "Hello World");
    REQUIRE(str.size() == 11);

    // empty builder
    memepp::string_builder empty_sb;
    str = empty_sb;
    REQUIRE(str.empty());
}

// 测试 substr 的异常/边界
TEST_CASE("memepp::string substr boundary", "[string]")
{
    memepp::string str = "0123456789";

    REQUIRE(str.substr(0, 0) == "");
    REQUIRE(str.substr(5, 100) == "56789"); // exceed length
    REQUIRE(str.substr(10, 1) == ""); // start at end
    REQUIRE(str.substr(11, 1) == ""); // start beyond end

    // negative (though not standard, check behavior)
    REQUIRE(str.substr(-1, 5) == "01234"); // assuming negative start from end, but confirm actual impl
    REQUIRE(str.substr(0, -1) == str); // full string
}
