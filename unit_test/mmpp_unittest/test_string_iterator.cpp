
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <string>

TEMPLATE_TEST_CASE("memepp::string begin/end", "[string]",
                   memepp::string, memepp::string_view)
{
    SECTION("length 10")
    {
        TestType obj = "0123456789";

        REQUIRE(*obj.begin()     == obj.at(0));
        REQUIRE(*(--obj.end())   == obj.at(9));
        REQUIRE(*obj.cbegin()    == obj.at(0));
        REQUIRE(*(--obj.cend())  == obj.at(9));
        REQUIRE(*obj.rbegin()    == obj.at(9));
        REQUIRE(*(--obj.rend())  == obj.at(0));
        REQUIRE(*obj.crbegin()   == obj.at(9));
        REQUIRE(*(--obj.crend()) == obj.at(0));

        REQUIRE(*std::prev(obj.end())   == obj.at(9));
        REQUIRE(*std::prev(obj.cend())  == obj.at(9));
        REQUIRE(*std::prev(obj.rend())  == obj.at(0));
        REQUIRE(*std::prev(obj.crend()) == obj.at(0));

        REQUIRE(std::begin(obj)   == obj.begin());
        REQUIRE(std::end(obj)     == obj.end());
        REQUIRE(std::cbegin(obj)  == obj.cbegin());
        REQUIRE(std::cend(obj)    == obj.cend());
        REQUIRE(std::rbegin(obj)  == obj.rbegin());
        REQUIRE(std::rend(obj)    == obj.rend());
        REQUIRE(std::crbegin(obj) == obj.crbegin());
        REQUIRE(std::crend(obj)   == obj.crend());

        REQUIRE(std::distance(obj.begin(), obj.end())     == 10);
        REQUIRE(std::distance(obj.cbegin(), obj.cend())   == 10);
        REQUIRE(std::distance(obj.rbegin(), obj.rend())   == 10);
        REQUIRE(std::distance(obj.crbegin(), obj.crend()) == 10);

        for (auto it = obj.begin(); it != obj.end(); ++it)
            REQUIRE(*it == obj.at(it - obj.begin()));

        for (auto it = obj.rbegin(); it != obj.rend(); ++it)
            REQUIRE(*it == obj.at(obj.size() - 1 - (it - obj.rbegin())));

        for (auto it = obj.cbegin(); it != obj.cend(); ++it)
            REQUIRE(*it == obj.at(it - obj.cbegin()));

        for (auto it = obj.crbegin(); it != obj.crend(); ++it)
            REQUIRE(*it == obj.at(obj.size() - 1 - (it - obj.crbegin())));
    }

    SECTION("length 1")
    {
        TestType obj = "0";

        REQUIRE(*obj.begin()     == obj.at(0));
        REQUIRE(*(--obj.end())   == obj.at(0));
        REQUIRE(*obj.cbegin()    == obj.at(0));
        REQUIRE(*(--obj.cend())  == obj.at(0));
        REQUIRE(*obj.rbegin()    == obj.at(0));
        REQUIRE(*(--obj.rend())  == obj.at(0));
        REQUIRE(*obj.crbegin()   == obj.at(0));
        REQUIRE(*(--obj.crend()) == obj.at(0));

        REQUIRE(obj.begin()     == --obj.end());
        REQUIRE(obj.cbegin()    == --obj.cend());
        REQUIRE(obj.rbegin()    == --obj.rend());
        REQUIRE(obj.crbegin()   == --obj.crend());
        REQUIRE(++obj.begin()   == obj.end());
        REQUIRE(++obj.cbegin()  == obj.cend());
        REQUIRE(++obj.rbegin()  == obj.rend());
        REQUIRE(++obj.crbegin() == obj.crend());

        REQUIRE(std::begin(obj)   == obj.begin());
        REQUIRE(std::end(obj)     == obj.end());
        REQUIRE(std::cbegin(obj)  == obj.cbegin());
        REQUIRE(std::cend(obj)    == obj.cend());
        REQUIRE(std::rbegin(obj)  == obj.rbegin());
        REQUIRE(std::rend(obj)    == obj.rend());
        REQUIRE(std::crbegin(obj) == obj.crbegin());
        REQUIRE(std::crend(obj)   == obj.crend());

        REQUIRE(std::next(obj.begin())   == obj.end());
        REQUIRE(std::next(obj.cbegin())  == obj.cend());
        REQUIRE(std::next(obj.rbegin())  == obj.rend());
        REQUIRE(std::next(obj.crbegin()) == obj.crend());
        REQUIRE(std::prev(obj.end())     == obj.begin());
        REQUIRE(std::prev(obj.cend())    == obj.cbegin());
        REQUIRE(std::prev(obj.rend())    == obj.rbegin());
        REQUIRE(std::prev(obj.crend())   == obj.crbegin());

        REQUIRE(std::distance(obj.begin(), obj.end())     == 1);
        REQUIRE(std::distance(obj.cbegin(), obj.cend())   == 1);
        REQUIRE(std::distance(obj.rbegin(), obj.rend())   == 1);
        REQUIRE(std::distance(obj.crbegin(), obj.crend()) == 1);
    }

    SECTION("length 0")
    {
        TestType obj = "";

        REQUIRE(obj.begin()   == obj.end());
        REQUIRE(obj.cbegin()  == obj.cend());
        REQUIRE(obj.rbegin()  == obj.rend());
        REQUIRE(obj.crbegin() == obj.crend());
    }
}

TEST_CASE("memepp::string iterator - rune iterator", "[string]")
{
    memepp::string str04 = u8"0123456789零一二三四五六七八九";
    auto rune_it04 = str04.to_rune_iterator(10);

    REQUIRE(rune_it04.is_valid() == true);
    REQUIRE(*rune_it04 == str04.at(10));
    REQUIRE(rune_it04.to_rune() == u8"零");
    REQUIRE(rune_it04.to_index().data() == &str04.at(10));

    rune_it04 = str04.to_rune_iterator(11);

    REQUIRE(rune_it04.is_valid() == false);
    REQUIRE(rune_it04.to_rune() == "");
    REQUIRE(rune_it04.to_index().data() == &str04.at(11));
    REQUIRE(rune_it04.to_index().size() == -1);
}

TEST_CASE("memepp::string iterator - user storage", "[string]")
{
    const mmint_t mediumLimit = MemeStringOption_getStorageMediumLimit();
    std::string data(mediumLimit + 100, 'I');
    memepp::string user_str = memepp::from(std::move(data));
    REQUIRE(user_str.storage_type() == memepp::string_storage_t::user);

    size_t count = 0;
    for (auto it = user_str.begin(); it != user_str.end(); ++it) {
        REQUIRE(*it == 'I');
        ++count;
    }
    REQUIRE(count == static_cast<size_t>(user_str.size()));
}

TEST_CASE("memepp::string iterator - view storage", "[string]")
{
    const char* src = "ABC";
    mmstrstk_t view_str;
    int rc = MemeStringViewUnsafeStack_init(&view_str, sizeof(view_str),
        reinterpret_cast<const uint8_t*>(src), -1);
    REQUIRE(rc == 0);
    REQUIRE(MemeString_storageType((mmstr_cptr_t)&view_str) == MemeString_UnsafeStorageType_view);

    memepp::string_view sv(
        reinterpret_cast<const char*>(MemeString_byteData((mmstr_cptr_t)&view_str)),
        MemeString_byteSize((mmstr_cptr_t)&view_str));
    size_t count = 0;
    for (auto it = sv.begin(); it != sv.end(); ++it) {
        ++count;
    }
    REQUIRE(count == 3);

    MemeStringStack_unInit(&view_str, sizeof(view_str));
}
