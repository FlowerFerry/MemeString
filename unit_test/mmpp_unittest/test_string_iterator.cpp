
#include <catch2/catch.hpp>

#include <memepp/string_view.hpp>
#include <memepp/string.hpp>

TEST_CASE("memepp::string - 46", "string begin/end")
{
    memepp::string str01 = "0123456789";
    memepp::string_view sv01 = str01;

    REQUIRE(*str01.begin()     == str01.at(0));
    REQUIRE(*(--str01.end())   == str01.at(9));
    REQUIRE(*str01.cbegin()    == str01.at(0));
    REQUIRE(*(--str01.cend())  == str01.at(9));
    REQUIRE(*str01.rbegin()    == str01.at(9));
    REQUIRE(*(--str01.rend())  == str01.at(0));
    REQUIRE(*str01.crbegin()   == str01.at(9));
    REQUIRE(*(--str01.crend()) == str01.at(0));

    REQUIRE(*sv01.begin()     == sv01.at(0));
    REQUIRE(*(--sv01.end())   == sv01.at(9));
    REQUIRE(*sv01.cbegin()    == sv01.at(0));
    REQUIRE(*(--sv01.cend())  == sv01.at(9));
    REQUIRE(*sv01.rbegin()    == sv01.at(9));
    REQUIRE(*(--sv01.rend())  == sv01.at(0));
    REQUIRE(*sv01.crbegin()   == sv01.at(9));
    REQUIRE(*(--sv01.crend()) == sv01.at(0));

    REQUIRE(*std::prev(str01.end())   == str01.at(9));
    REQUIRE(*std::prev(str01.cend())  == str01.at(9));
    REQUIRE(*std::prev(str01.rend())  == str01.at(0));
    REQUIRE(*std::prev(str01.crend()) == str01.at(0));

    REQUIRE(*std::prev(sv01.end())   == sv01.at(9));
    REQUIRE(*std::prev(sv01.cend())  == sv01.at(9));
    REQUIRE(*std::prev(sv01.rend())  == sv01.at(0));
    REQUIRE(*std::prev(sv01.crend()) == sv01.at(0));

    REQUIRE(std::begin(str01)  == str01.begin());
    REQUIRE(std::end(str01)    == str01.end());
    REQUIRE(std::cbegin(str01) == str01.cbegin());
    REQUIRE(std::cend(str01)   == str01.cend());
    REQUIRE(std::rbegin(str01) == str01.rbegin());
    REQUIRE(std::rend(str01)   == str01.rend());
    REQUIRE(std::crbegin(str01)== str01.crbegin());
    REQUIRE(std::crend(str01)  == str01.crend());

    REQUIRE(std::begin(sv01)   == sv01.begin());
    REQUIRE(std::end(sv01)     == sv01.end());
    REQUIRE(std::cbegin(sv01)  == sv01.cbegin());
    REQUIRE(std::cend(sv01)    == sv01.cend());
    REQUIRE(std::rbegin(sv01)  == sv01.rbegin());
    REQUIRE(std::rend(sv01)    == sv01.rend());
    REQUIRE(std::crbegin(sv01) == sv01.crbegin());
    REQUIRE(std::crend(sv01)   == sv01.crend());

    REQUIRE(std::distance(str01.begin(), str01.end())     == 10);
    REQUIRE(std::distance(str01.cbegin(), str01.cend())   == 10);
    REQUIRE(std::distance(str01.rbegin(), str01.rend())   == 10);
    REQUIRE(std::distance(str01.crbegin(), str01.crend()) == 10);

    REQUIRE(std::distance(sv01.begin(), sv01.end())       == 10);
    REQUIRE(std::distance(sv01.cbegin(), sv01.cend())     == 10);
    REQUIRE(std::distance(sv01.rbegin(), sv01.rend())     == 10);
    REQUIRE(std::distance(sv01.crbegin(), sv01.crend())   == 10);

    for (auto it = str01.begin(); it != str01.end(); ++it)
        REQUIRE(*it == str01.at(it - str01.begin()));
    
    for (auto it = str01.rbegin(); it != str01.rend(); ++it)
        REQUIRE(*it == str01.at(str01.size() - 1 - (it - str01.rbegin())));
    
    for (auto it = str01.cbegin(); it != str01.cend(); ++it)
        REQUIRE(*it == str01.at(it - str01.cbegin()));

    for (auto it = str01.crbegin(); it != str01.crend(); ++it)
        REQUIRE(*it == str01.at(str01.size() - 1 - (it - str01.crbegin())));

    for (auto it = sv01.begin(); it != sv01.end(); ++it)
        REQUIRE(*it == sv01.at(it - sv01.begin()));
    
    for (auto it = sv01.rbegin(); it != sv01.rend(); ++it)
        REQUIRE(*it == sv01.at(sv01.size() - 1 - (it - sv01.rbegin())));

    for (auto it = sv01.cbegin(); it != sv01.cend(); ++it)
        REQUIRE(*it == sv01.at(it - sv01.cbegin()));
    
    for (auto it = sv01.crbegin(); it != sv01.crend(); ++it)
        REQUIRE(*it == sv01.at(sv01.size() - 1 - (it - sv01.crbegin())));

    memepp::string str02 = "0";
    memepp::string_view sv02 = str02;

    REQUIRE(*str02.begin()     == str02.at(0));
    REQUIRE(*(--str02.end())   == str02.at(0));
    REQUIRE(*str02.cbegin()    == str02.at(0));
    REQUIRE(*(--str02.cend())  == str02.at(0));
    REQUIRE(*str02.rbegin()    == str02.at(0));
    REQUIRE(*(--str02.rend())  == str02.at(0));
    REQUIRE(*str02.crbegin()   == str02.at(0));
    REQUIRE(*(--str02.crend()) == str02.at(0));
    REQUIRE(str02.begin()      == --str02.end());
    REQUIRE(str02.cbegin()     == --str02.cend());
    REQUIRE(str02.rbegin()     == --str02.rend());
    REQUIRE(str02.crbegin()    == --str02.crend());
    REQUIRE(++str02.begin()    == str02.end());
    REQUIRE(++str02.cbegin()   == str02.cend());
    REQUIRE(++str02.rbegin()   == str02.rend());
    REQUIRE(++str02.crbegin()  == str02.crend());

    REQUIRE(*sv02.begin()     == sv02.at(0));
    REQUIRE(*(--sv02.end())   == sv02.at(0));
    REQUIRE(*sv02.cbegin()    == sv02.at(0));
    REQUIRE(*(--sv02.cend())  == sv02.at(0));
    REQUIRE(*sv02.rbegin()    == sv02.at(0));
    REQUIRE(*(--sv02.rend())  == sv02.at(0));
    REQUIRE(*sv02.crbegin()   == sv02.at(0));
    REQUIRE(*(--sv02.crend()) == sv02.at(0));
    REQUIRE(sv02.begin()      == --sv02.end());
    REQUIRE(sv02.cbegin()     == --sv02.cend());
    REQUIRE(sv02.rbegin()     == --sv02.rend());
    REQUIRE(sv02.crbegin()    == --sv02.crend());
    REQUIRE(++sv02.begin()    == sv02.end());
    REQUIRE(++sv02.cbegin()   == sv02.cend());
    REQUIRE(++sv02.rbegin()   == sv02.rend());
    REQUIRE(++sv02.crbegin()  == sv02.crend());

    REQUIRE(std::begin(str02)  == str02.begin());
    REQUIRE(std::end(str02)    == str02.end());
    REQUIRE(std::cbegin(str02) == str02.cbegin());
    REQUIRE(std::cend(str02)   == str02.cend());
    REQUIRE(std::rbegin(str02) == str02.rbegin());
    REQUIRE(std::rend(str02)   == str02.rend());
    REQUIRE(std::crbegin(str02)== str02.crbegin());
    REQUIRE(std::crend(str02)  == str02.crend());

    REQUIRE(std::begin(sv02)   == sv02.begin());
    REQUIRE(std::end(sv02)     == sv02.end());
    REQUIRE(std::cbegin(sv02)  == sv02.cbegin());
    REQUIRE(std::cend(sv02)    == sv02.cend());
    REQUIRE(std::rbegin(sv02)  == sv02.rbegin());
    REQUIRE(std::rend(sv02)    == sv02.rend());
    REQUIRE(std::crbegin(sv02) == sv02.crbegin());
    REQUIRE(std::crend(sv02)   == sv02.crend());

    REQUIRE(std::next(str02.begin())   == str02.end());
    REQUIRE(std::next(str02.cbegin())  == str02.cend());
    REQUIRE(std::next(str02.rbegin())  == str02.rend());
    REQUIRE(std::next(str02.crbegin()) == str02.crend());
    REQUIRE(std::prev(str02.end())     == str02.begin());
    REQUIRE(std::prev(str02.cend())    == str02.cbegin());
    REQUIRE(std::prev(str02.rend())    == str02.rbegin());
    REQUIRE(std::prev(str02.crend())   == str02.crbegin());

    REQUIRE(std::next(sv02.begin())   == sv02.end());
    REQUIRE(std::next(sv02.cbegin())  == sv02.cend());
    REQUIRE(std::next(sv02.rbegin())  == sv02.rend());
    REQUIRE(std::next(sv02.crbegin()) == sv02.crend());
    REQUIRE(std::prev(sv02.end())     == sv02.begin());
    REQUIRE(std::prev(sv02.cend())    == sv02.cbegin());
    REQUIRE(std::prev(sv02.rend())    == sv02.rbegin());
    REQUIRE(std::prev(sv02.crend())   == sv02.crbegin());

    REQUIRE(std::distance(str02.begin(), str02.end())     == 1);
    REQUIRE(std::distance(str02.cbegin(), str02.cend())   == 1);
    REQUIRE(std::distance(str02.rbegin(), str02.rend())   == 1);
    REQUIRE(std::distance(str02.crbegin(), str02.crend()) == 1);

    REQUIRE(std::distance(sv02.begin(), sv02.end())       == 1);
    REQUIRE(std::distance(sv02.cbegin(), sv02.cend())     == 1);
    REQUIRE(std::distance(sv02.rbegin(), sv02.rend())     == 1);
    REQUIRE(std::distance(sv02.crbegin(), sv02.crend())   == 1);

    memepp::string str03 = "";
    memepp::string_view sv03 = str03;
    REQUIRE(str03.begin()   == str03.end());
    REQUIRE(str03.cbegin()  == str03.cend());
    REQUIRE(str03.rbegin()  == str03.rend());
    REQUIRE(str03.crbegin() == str03.crend());

    REQUIRE(sv03.begin()    == sv03.end());
    REQUIRE(sv03.cbegin()   == sv03.cend());
    REQUIRE(sv03.rbegin()   == sv03.rend());
    REQUIRE(sv03.crbegin()  == sv03.crend());

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
