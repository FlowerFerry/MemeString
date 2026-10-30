
#include <catch2/catch.hpp>

#include <mego/predef/lang/version.h>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>
#include <memepp/convert/std/string.hpp>
#include <iterator>
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
        // std::cbegin/cend/rbegin/rend/crbegin/crend arrived in C++14.  At C++11
        // only the const-iterator aliases can be checked portably: the reverse
        // ones return memepp::reverse_iterator / memepp::const_reverse_iterator,
        // which do not compare against std::reverse_iterator.
#if MG_LANG__CXX14_AVAIL
        REQUIRE(std::cbegin(obj)  == obj.cbegin());
        REQUIRE(std::cend(obj)    == obj.cend());
        REQUIRE(std::rbegin(obj)  == obj.rbegin());
        REQUIRE(std::rend(obj)    == obj.rend());
        REQUIRE(std::crbegin(obj) == obj.crbegin());
        REQUIRE(std::crend(obj)   == obj.crend());
#else
        REQUIRE(obj.cbegin() == obj.begin());
        REQUIRE(obj.cend()   == obj.end());
#endif

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

TEST_CASE("memepp::string iterator - random access", "[string]")
{
    memepp::string s = "0123456789";
    auto it = s.begin();

    REQUIRE(*(it + 3) == '3');
    REQUIRE(it[5]     == '5');
    REQUIRE(*(it + 9) == '9');

    REQUIRE(it          <  it + 1);
    REQUIRE(it + 1      >  it);
    REQUIRE(it          <= it);
    REQUIRE(it          >= it);
    REQUIRE_FALSE(it    == it + 1);
    REQUIRE(it + 3 - it == 3);

    auto it2 = it;
    it2 += 4;
    REQUIRE(*it2 == '4');
    it2 -= 2;
    REQUIRE(*it2 == '2');

    auto rit = s.rbegin();
    REQUIRE(*(rit + 1) == '8');
    REQUIRE(rit[2]     == '7');
    REQUIRE(rit       < rit + 1);
}

TEST_CASE("memepp::string iterator - rune_begin/rune_end", "[string]")
{
    memepp::string s = u8"a零b";

    auto rb = s.rune_begin();
    auto re = s.rune_end();
    REQUIRE(rb != re);
    REQUIRE(rb.to_rune() == "a");
    REQUIRE(std::distance(rb, re) == 3);

    auto last = re;
    REQUIRE((--last).to_rune() == "b");

    auto rcb = s.rune_cbegin();
    auto rce = s.rune_cend();
    REQUIRE(rcb != rce);
    REQUIRE(rcb.to_rune() == "a");
    REQUIRE(std::distance(rcb, rce) == 3);

    memepp::string empty_str = "";
    REQUIRE(empty_str.rune_begin() == empty_str.rune_end());
    REQUIRE(empty_str.rune_cbegin() == empty_str.rune_cend());
}

TEST_CASE("memepp::string iterator - large storage", "[string]")
{
    memepp::string small = "0123456789";
    memepp::string large = small.to_large();
    REQUIRE(large.storage_type() == memepp::string_storage_t::large);

    REQUIRE(*large.begin()    == '0');
    REQUIRE(*(--large.end())  == '9');
    REQUIRE(*large.rbegin()   == '9');
    REQUIRE(std::distance(large.begin(), large.end()) == 10);

    size_t count = 0;
    for (auto it = large.begin(); it != large.end(); ++it) {
        REQUIRE(*it == small.at(count));
        ++count;
    }
    REQUIRE(count == 10);
}

TEST_CASE("memepp::string iterator - copy independence", "[string]")
{
    memepp::string original = "hello";
    memepp::string shared = original.to_shared_storage();
    REQUIRE(shared.storage_type() == memepp::string_storage_t::large);

    auto it = shared.begin();
    REQUIRE(*it == 'h');

    memepp::string copy = shared;

    // Iterators obtained before the copy remain valid afterwards.
    REQUIRE(*it == 'h');
    REQUIRE(*(--shared.end()) == 'o');

    size_t count_shared = 0;
    for (auto i = shared.begin(); i != shared.end(); ++i) {
        REQUIRE(*i == *it);
        ++count_shared;
        ++it;
    }
    REQUIRE(count_shared == 5);

    size_t count_copy = 0;
    for (auto i = copy.begin(); i != copy.end(); ++i)
        ++count_copy;
    REQUIRE(count_copy == 5);
    REQUIRE(*copy.begin() == 'h');
}
