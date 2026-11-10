#include <catch2/catch.hpp>

#include <memepp/searcher.hpp>
#include <memepp/string.hpp>
#include <cctype>
#include <cstring>
#include <string>
#include <type_traits>

/* --------------------------------------------------------------------- */
/* C API                                                                  */
/* --------------------------------------------------------------------- */

TEST_CASE("MemeSearcherStack: basic find", "[searcher]")
{
    const char* hay    = "The quick brown fox jumps over the lazy dog.";
    const char* needle = "fox";

    MemeSearcherStack_t s;
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>(needle),
        static_cast<MemeInteger_t>(std::strlen(needle)),
        MemeFlag_CaseSensitive) == 0);

    const auto hn = static_cast<MemeInteger_t>(std::strlen(hay));
    const auto hb = reinterpret_cast<const MemeByte_t*>(hay);

    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 0) == 16);
    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 20) == -1);

    REQUIRE(MemeSearcherStack_unInit(&s) == 0);
}

TEST_CASE("MemeSearcherStack: offset handling", "[searcher]")
{
    const char* hay = "abcabc";

    MemeSearcherStack_t s;
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>("abc"), 3,
        MemeFlag_CaseSensitive) == 0);

    const auto hn = static_cast<MemeInteger_t>(std::strlen(hay));
    const auto hb = reinterpret_cast<const MemeByte_t*>(hay);

    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 0) == 0);
    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 1) == 3);
    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 6) == -1);

    /* Negative / out-of-range offset. */
    REQUIRE(MemeSearcherStack_find(&s, hb, hn, -1) == -1);
    REQUIRE(MemeSearcherStack_find(&s, hb, hn, 99) == -1);

    MemeSearcherStack_unInit(&s);
}

TEST_CASE("MemeSearcherStack: case-insensitive", "[searcher]")
{
    const char* hay = "HeLLo WoRLd!";

    MemeSearcherStack_t s;
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>("world"), 5,
        MemeFlag_AllInsensitive) == 0);

    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>(hay),
        static_cast<MemeInteger_t>(std::strlen(hay)), 0) == 6);

    MemeSearcherStack_unInit(&s);
}

TEST_CASE("MemeSearcherStack: invalid inputs", "[searcher]")
{
    MemeSearcherStack_t s;

    /* NULL needle / zero length. */
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE, nullptr, 3, MemeFlag_CaseSensitive) != 0);
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>(""), 0,
        MemeFlag_CaseSensitive) != 0);

    /* Undersized object. */
    REQUIRE(MemeSearcherStack_init(
        &s, sizeof(MemeSearcherStack_t) / 2,
        reinterpret_cast<const MemeByte_t*>("x"), 1,
        MemeFlag_CaseSensitive) != 0);

    /* Valid object, then NULL haystack. */
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>("x"), 1,
        MemeFlag_CaseSensitive) == 0);
    REQUIRE(MemeSearcherStack_find(&s, nullptr, 10, 0) == -1);

    /* Haystack shorter than the needle. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>("y"), 1, 0) == -1);

    MemeSearcherStack_unInit(&s);

    /* unInit on an already-released object is a no-op. */
    REQUIRE(MemeSearcherStack_unInit(&s) == 0);

    /* Uninitialised object: find returns -1 and does not crash. */
    MemeSearcherStack_t fresh;
    std::memset(&fresh, 0, sizeof(fresh));
    REQUIRE(MemeSearcherStack_find(
        &fresh, reinterpret_cast<const MemeByte_t*>("abc"), 3, 0) == -1);
}

TEST_CASE("MemeSearcherStack: move is O(1) and leaves the source empty", "[searcher]")
{
    const char* hay = "the quick brown fox jumps over the lazy dog";

    /* FastScan case: needle stored in the object's own inline_ area. */
    {
        MemeSearcherStack_t a;
        REQUIRE(MemeSearcherStack_init(
            &a, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const MemeByte_t*>("brown"), 5,
            MemeFlag_CaseSensitive) == 0);

        MemeSearcherStack_t b;
        REQUIRE(MemeSearcherStack_move(
            &b, MMSEARCHER__OBJ_SIZE, &a) == 0);

        /* The moved-to object finds the needle. */
        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(hay),
            static_cast<MemeInteger_t>(std::strlen(hay)), 0) == 10);

        /* The source no longer reports anything. */
        REQUIRE(MemeSearcherStack_find(
            &a, reinterpret_cast<const MemeByte_t*>(hay),
            static_cast<MemeInteger_t>(std::strlen(hay)), 0) == -1);

        /* Releasing the emptied source must not free the destination's data. */
        REQUIRE(MemeSearcherStack_unInit(&a) == 0);

        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(hay),
            static_cast<MemeInteger_t>(std::strlen(hay)), 0) == 10);

        MemeSearcherStack_unInit(&b);
    }

    /* Boyer-Moore case with b_width == 1: the bad-char table lives in
     * table live inside the object, so the relocation path is exercised. */
    {
        std::string needle(200, 'a');
        std::string big = "xx" + needle + "yy";
        std::string miss(200, 'b');

        MemeSearcherStack_t a;
        REQUIRE(MemeSearcherStack_init(
            &a, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const MemeByte_t*>(needle.data()),
            static_cast<MemeInteger_t>(needle.size()),
            MemeFlag_AllInsensitive) == 0);

        MemeSearcherStack_t b;
        REQUIRE(MemeSearcherStack_move(
            &b, MMSEARCHER__OBJ_SIZE, &a) == 0);

        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(big.data()),
            static_cast<MemeInteger_t>(big.size()), 0) == 2);
        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(miss.data()),
            static_cast<MemeInteger_t>(miss.size()), 0) == -1);

        MemeSearcherStack_unInit(&a);   /* must not corrupt b */

        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(big.data()),
            static_cast<MemeInteger_t>(big.size()), 0) == 2);

        MemeSearcherStack_unInit(&b);
    }

    /* Boyer-Moore case with b_width > 1: tables live on the heap. */
    {
        std::string needle(300, 'c');
        std::string big = "zz" + needle + "ww";

        MemeSearcherStack_t a;
        REQUIRE(MemeSearcherStack_init(
            &a, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const MemeByte_t*>(needle.data()),
            static_cast<MemeInteger_t>(needle.size()),
            MemeFlag_AllInsensitive) == 0);

        MemeSearcherStack_t b;
        REQUIRE(MemeSearcherStack_move(
            &b, MMSEARCHER__OBJ_SIZE, &a) == 0);

        MemeSearcherStack_unInit(&a);

        REQUIRE(MemeSearcherStack_find(
            &b, reinterpret_cast<const MemeByte_t*>(big.data()),
            static_cast<MemeInteger_t>(big.size()), 0) == 2);

        MemeSearcherStack_unInit(&b);
    }
}

TEST_CASE("MemeSearcherStack: density tracking", "[searcher]")
{
    const char* hit_s  = "hello world";
    const char* miss_s = "goodbye";
    const auto hh = reinterpret_cast<const MemeByte_t*>(hit_s);
    const auto hm = reinterpret_cast<const MemeByte_t*>(miss_s);
    const auto nh = static_cast<MemeInteger_t>(std::strlen(hit_s));
    const auto nm = static_cast<MemeInteger_t>(std::strlen(miss_s));

    MemeSearcherStack_t s;
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>("world"), 5,
        MemeFlag_CaseSensitive) == 0);

    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Unknown);

    /* Below the 4-call floor. */
    for (int i = 0; i < 3; ++i)
        MemeSearcherStack_find(&s, hh, nh, 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Unknown);

    /* 4/4 = 100 % -> Frequent. */
    MemeSearcherStack_find(&s, hh, nh, 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Frequent);

    /* 4/6 -> Common. */
    MemeSearcherStack_find(&s, hm, nm, 0);
    MemeSearcherStack_find(&s, hm, nm, 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Common);

    /* 4/10 -> Sparse. */
    for (int i = 0; i < 4; ++i)
        MemeSearcherStack_find(&s, hm, nm, 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Sparse);

    /* 4/18 -> Rare. */
    for (int i = 0; i < 8; ++i)
        MemeSearcherStack_find(&s, hm, nm, 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Rare);

    REQUIRE(MemeSearcherStack_resetDensity(&s) == 0);
    REQUIRE(MemeSearcherStack_density(&s) == MemeSearcherDensity_Unknown);

    MemeSearcherStack_unInit(&s);
}

/* --------------------------------------------------------------------- */
/* Table-width selection                                                  */
/* --------------------------------------------------------------------- */

TEST_CASE("MemeSearcherStack: bad-char table width selection", "[searcher]")
{
    /*
     * The table width is internal state, so this verifies behaviour rather
     * than poking at struct offsets: each length must produce a searcher that
     * finds its own needle and misses a perturbed one.
     *
     * 255 / 256 and 65535 / 65536 straddle the uint8 / uint16 / uint32
     * thresholds.  These lengths only reach the Boyer-Moore path when the
     * search is case-insensitive (MEME_IMPL_SHORT_NEEDLE_MAX is 32 and the
     * searcher threshold is MEME_IMPL_SEARCHER_SHORT_NEEDLE_MAX (8), and).
     */
    auto roundtrip = [](size_t len) {
        std::string needle(len, 'a');
        std::string hay = "xx" + needle + "yy";

        MemeSearcherStack_t s;
        int rc = MemeSearcherStack_init(
            &s, MMSEARCHER__OBJ_SIZE,
            reinterpret_cast<const MemeByte_t*>(needle.data()),
            static_cast<MemeInteger_t>(len),
            MemeFlag_AllInsensitive);
        REQUIRE(rc == 0);

        REQUIRE(MemeSearcherStack_find(
            &s, reinterpret_cast<const MemeByte_t*>(hay.data()),
            static_cast<MemeInteger_t>(hay.size()), 0) == 2);

        /* A haystack without the needle must miss. */
        std::string other(len, 'b');
        REQUIRE(MemeSearcherStack_find(
            &s, reinterpret_cast<const MemeByte_t*>(other.data()),
            static_cast<MemeInteger_t>(other.size()), 0) == -1);

        MemeSearcherStack_unInit(&s);
    };

    roundtrip(255);    /* uint8_t bad-char table, stored inline */
    roundtrip(256);    /* uint16_t table, on the heap          */
    roundtrip(65535);  /* uint16_t table                       */
    roundtrip(65536);  /* uint32_t table                       */
}

TEST_CASE("MemeSearcherStack: initByOther is a deep copy", "[searcher]")
{
    const char* hay = "the quick brown fox";

    MemeSearcherStack_t a;
    REQUIRE(MemeSearcherStack_init(
        &a, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>("brown"), 5,
        MemeFlag_CaseSensitive) == 0);

    MemeSearcherStack_t b;
    REQUIRE(MemeSearcherStack_initByOther(
        &b, MMSEARCHER__OBJ_SIZE, &a) == 0);

    /* Releasing the source must not affect the copy. */
    MemeSearcherStack_unInit(&a);

    REQUIRE(MemeSearcherStack_find(
        &b, reinterpret_cast<const MemeByte_t*>(hay),
        static_cast<MemeInteger_t>(std::strlen(hay)), 0) == 10);

    MemeSearcherStack_unInit(&b);
}

/* --------------------------------------------------------------------- */
/* C++ wrapper                                                            */
/* --------------------------------------------------------------------- */

TEST_CASE("memepp::searcher: basic usage", "[searcher]")
{
    memepp::string hay = "The quick brown fox jumps over the lazy dog.";

    memepp::searcher srch("fox");
    REQUIRE(srch.valid());
    REQUIRE(srch.find(hay) == 16);
    REQUIRE(srch.find(hay, 20) == -1);
}

TEST_CASE("memepp::searcher: case-insensitive", "[searcher]")
{
    memepp::string hay = "HeLLo WoRLd!";
    memepp::searcher srch("WORLD", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(srch.valid());
    REQUIRE(srch.find(hay) == 6);
}

TEST_CASE("memepp::searcher: constructors", "[searcher]")
{
    memepp::string hay = "abcabc";

    memepp::searcher from_cstr("abc");
    REQUIRE(from_cstr.valid());
    REQUIRE(from_cstr.find(hay) == 0);

#if MG_LANG__CXX17_AVAIL
    memepp::searcher from_sv(std::string_view("abc"));
    REQUIRE(from_sv.valid());
    REQUIRE(from_sv.find(hay) == 0);
#endif

    memepp::searcher from_bytes(
        reinterpret_cast<const MemeByte_t*>("abc"), 3);
    REQUIRE(from_bytes.valid());
    REQUIRE(from_bytes.find(hay) == 0);

    /* Empty needle is rejected. */
    memepp::searcher empty("");
    REQUIRE(!empty.valid());
    REQUIRE(empty.find(hay) == -1);
}

TEST_CASE("memepp::searcher: move semantics", "[searcher]")
{
    memepp::string hay = "hello world";

    memepp::searcher a("world");
    REQUIRE(a.valid());
    REQUIRE(a.find(hay) == 6);

    memepp::searcher b(std::move(a));
    REQUIRE(!a.valid());
    REQUIRE(b.valid());
    REQUIRE(b.find(hay) == 6);

    memepp::searcher c("hello");
    c = std::move(b);
    REQUIRE(!b.valid());
    REQUIRE(c.find(hay) == 6);
}

TEST_CASE("memepp::searcher: copy is a deep copy", "[searcher]")
{
    memepp::string hay = "the quick brown fox jumps over the lazy dog";

    memepp::searcher a("brown");
    REQUIRE(a.valid());
    REQUIRE(a.find(hay) == 10);

    /* Copy construction: both must find, and releasing one must not affect
     * the other (initByOther rebuilds the tables). */
    memepp::searcher b(a);
    REQUIRE(b.valid());
    REQUIRE(b.find(hay) == 10);
    REQUIRE(a.find(hay) == 10);

    /* Copy assignment over an existing searcher. */
    memepp::searcher c("fox");
    REQUIRE(c.find(hay) == 16);
    c = a;
    REQUIRE(c.valid());
    REQUIRE(c.find(hay) == 10);

    /* The original still works after c was assigned and destroyed. */
    {
        memepp::searcher tmp(b);
        REQUIRE(tmp.find(hay) == 10);
    }
    REQUIRE(b.find(hay) == 10);

    /* Copying an invalid searcher yields an invalid one. */
    memepp::searcher empty("");
    REQUIRE(!empty.valid());
    memepp::searcher empty_copy(empty);
    REQUIRE(!empty_copy.valid());
    REQUIRE(empty_copy.find(hay) == -1);
}

TEST_CASE("memepp::searcher: copy of a Boyer-Moore instance", "[searcher]")
{
    /* Long, case-insensitive needle -> Boyer-Moore with heap tables. */
    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";
    std::string miss(200, 'b');

    memepp::searcher a(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(a.valid());

    memepp::searcher b(a);
    REQUIRE(b.valid());

    REQUIRE(b.find(
        reinterpret_cast<const MemeByte_t*>(big.data()),
        static_cast<mmint_t>(big.size())) == 2);
    REQUIRE(a.find(
        reinterpret_cast<const MemeByte_t*>(miss.data()),
        static_cast<mmint_t>(miss.size())) == -1);

    /* Density counters are per-object, not shared with the source. */
    REQUIRE(a.density() == memepp::search_density_t::unknown);
    REQUIRE(b.density() == memepp::search_density_t::unknown);
}

TEST_CASE("memepp::searcher: density", "[searcher]")
{
    memepp::searcher srch("fox");
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";

    REQUIRE(srch.density() == memepp::search_density_t::unknown);

    for (int i = 0; i < 3; ++i) srch.find(hit);
    REQUIRE(srch.density() == memepp::search_density_t::unknown);

    srch.find(hit);                     /* 4/4 = 100 % */
    REQUIRE(srch.density() == memepp::search_density_t::frequent);

    for (int i = 0; i < 4; ++i) srch.find(miss);
    REQUIRE(srch.density() == memepp::search_density_t::common);  /* 4/8 = 50 % */

    srch.reset_density();
    REQUIRE(srch.density() == memepp::search_density_t::unknown);
}

/* --------------------------------------------------------------------- */
/* Gaps: constructors, find() overloads, lifecycle edge cases             */
/* --------------------------------------------------------------------- */

TEST_CASE("memepp::searcher: default constructor", "[searcher]")
{
    memepp::searcher srch;
    REQUIRE(!srch.valid());
    REQUIRE(srch.density() == memepp::search_density_t::unknown);
    REQUIRE(srch.find("abc") == -1);

    /* native_handle() must expose a zeroed object, not stack garbage. */
    const MemeSearcherStack_t* h = srch.native_handle();
    REQUIRE(h != nullptr);
    for (size_t i = 0; i < sizeof(MemeSearcherStack_t); ++i)
        REQUIRE(reinterpret_cast<const unsigned char*>(h)[i] == 0);
    REQUIRE(srch.error_code() == MGEC__OK);
    REQUIRE(srch.state() == memepp::searcher_state_t::empty);
}

TEST_CASE("memepp::searcher: memepp::string_view constructor", "[searcher]")
{
    memepp::string hay = "abcabc";

    memepp::string_view nv("abc", 3);
    memepp::searcher s(nv);
    REQUIRE(s.valid());
    REQUIRE(s.find(hay) == 0);

    memepp::string_view nvi("ABC", 3);
    memepp::searcher si(nvi, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(si.valid());
    REQUIRE(si.find(hay) == 0);

    /* Empty view is rejected. */
    memepp::string_view ev("", 0);
    memepp::searcher se(ev);
    REQUIRE(!se.valid());
}

TEST_CASE("memepp::searcher: constructor failure inputs", "[searcher]")
{
    memepp::string hay = "abc";

    /* nullptr C string. */
    memepp::searcher c1(static_cast<const char*>(nullptr));
    REQUIRE(!c1.valid());
    REQUIRE(c1.find(hay) == -1);

    /* nullptr raw bytes, even with a length. */
    memepp::searcher c2(static_cast<const MemeByte_t*>(nullptr), 3);
    REQUIRE(!c2.valid());

    /* Zero length. */
    memepp::searcher c3(reinterpret_cast<const MemeByte_t*>("abc"), 0);
    REQUIRE(!c3.valid());

    /* Negative length. */
    memepp::searcher c4(reinterpret_cast<const MemeByte_t*>("abc"), -1);
    REQUIRE(!c4.valid());

    /* Empty std::string_view and empty memepp::string_view.
     * Braces, not parens: `T v(std::string_view())` declares a function. */
#if MG_LANG__CXX17_AVAIL
    memepp::searcher c5{std::string_view()};
    REQUIRE(!c5.valid());
#endif
    memepp::searcher c6{memepp::string_view()};
    REQUIRE(!c6.valid());

    /* A failed constructor leaves a well-defined handle: everything zeroed
     * except last_errc, which records why it failed. */
    const unsigned char* raw =
        reinterpret_cast<const unsigned char*>(c1.native_handle());
    size_t nonzero = 0;
    for (size_t i = 0; i < sizeof(MemeSearcherStack_t); ++i)
        if (raw[i] != 0) ++nonzero;
    REQUIRE(nonzero > 0);            /* last_errc is set  */
    REQUIRE(nonzero <= sizeof(int));  /* nothing else is  */
    REQUIRE(c1.error_code() == MGEC__INVAL);
    REQUIRE(MemeSearcherStack_needleSize(c1.native_handle()) == 0);
    REQUIRE(MemeSearcherStack_state(c1.native_handle())
            == MemeSearcherState_Failed);
}

TEST_CASE("memepp::searcher: find(memepp::string_view)", "[searcher]")
{
    memepp::searcher s("brown");
    const char* hay = "the quick brown fox";
    memepp::string_view hv(hay, std::strlen(hay));

    REQUIRE(s.find(hv) == 10);
    REQUIRE(s.find(hv, 11) == -1);
    REQUIRE(s.find(memepp::string_view(), 0) == -1);
}

TEST_CASE("memepp::searcher: find(const char*) NUL-terminated", "[searcher]")
{
    memepp::searcher s("brown");
    const char* hay = "the quick brown fox";

    REQUIRE(s.find(hay) == 10);
    REQUIRE(s.find(hay, 11) == -1);
    REQUIRE(s.find(hay, 10) == 10);      /* hit exactly at the offset */
    REQUIRE(s.find("", 0) == -1);        /* empty haystack            */
    REQUIRE(s.find(hay, -1) == -1);      /* negative offset           */
    REQUIRE(s.find(hay, 999) == -1);     /* offset past the end       */

    /* nullptr haystack. */
    REQUIRE(s.find(static_cast<const char*>(nullptr)) == -1);

    /* Invalid searcher. */
    memepp::searcher inv;
    REQUIRE(inv.find(hay) == -1);
}

TEST_CASE("memepp::searcher: find(const char*) tracks density", "[searcher]")
{
    /* The NUL-terminated path returns early in the C layer; the density
     * counters must still advance, otherwise this overload silently
     * degrades the adaptive algorithm selection. */
    memepp::searcher s("fox");
    for (int i = 0; i < 4; ++i)
        REQUIRE(s.find("the quick brown fox") == 16);
    REQUIRE(s.density() == memepp::search_density_t::frequent);
}

TEST_CASE("memepp::searcher: find(raw bytes) coverage", "[searcher]")
{
    memepp::searcher short_n(reinterpret_cast<const MemeByte_t*>("abc"), 3);
    memepp::searcher long_n(
        reinterpret_cast<const MemeByte_t*>(
            "abcdefghijklmnopqrstuvwxyz0123456789"), 36,
        memepp::case_sensitivity_t::all_insensitive);

    auto B = [](const char* p) { return reinterpret_cast<const MemeByte_t*>(p); };

    /* Short needle on the FastScan path. */
    REQUIRE(short_n.find(B("xxabc"), 5) == 2);
    REQUIRE(short_n.find(B("xxabc"), 5, 3) == -1);

    /* Long needle on the Boyer-Moore path. */
    REQUIRE(long_n.find(B("zzabcdefghijklmnopqrstuvwxyz0123456789zz"), 40) == 2);
    REQUIRE(long_n.find(B("short"), 5) == -1);      /* haystack < needle */

    /* nullptr / zero length / negative length. */
    REQUIRE(short_n.find(static_cast<const MemeByte_t*>(nullptr), 5) == -1);
    REQUIRE(short_n.find(B("abc"), 0) == -1);
    REQUIRE(short_n.find(B("abc"), -1) == -1);

    /* Offset boundaries. */
    REQUIRE(short_n.find(B("abcabc"), 6, 0) == 0);
    REQUIRE(short_n.find(B("abcabc"), 6, 3) == 3);
    REQUIRE(short_n.find(B("abcabc"), 6, 6) == -1);
    REQUIRE(short_n.find(B("abcabc"), 6, 7) == -1);
    REQUIRE(short_n.find(B("abcabc"), 6, -1) == -1);

    /* Embedded NUL in an explicitly-sized haystack is just a byte. */
    const MemeByte_t with_nul[] = { 'x', 0, 'a', 'b', 'c' };
    REQUIRE(short_n.find(with_nul, 5) == 2);
}

TEST_CASE("memepp::searcher: find(memepp::string) offset boundaries", "[searcher]")
{
    memepp::searcher s("abc");
    memepp::string hay = "abcabc";

    REQUIRE(s.find(hay, 0) == 0);
    REQUIRE(s.find(hay, 1) == 3);
    REQUIRE(s.find(hay, 3) == 3);
    REQUIRE(s.find(hay, 4) == -1);
    REQUIRE(s.find(hay, 6) == -1);      /* offset == size  */
    REQUIRE(s.find(hay, 7) == -1);      /* offset > size   */
    REQUIRE(s.find(hay, -1) == -1);     /* negative offset */

    /* Empty haystack. */
    memepp::string empty;
    REQUIRE(s.find(empty, 0) == -1);
}

TEST_CASE("memepp::searcher: swap", "[searcher]")
{
    memepp::string hay = "the quick brown fox";
    memepp::searcher a("brown");
    memepp::searcher b("fox");

    REQUIRE(a.find(hay) == 10);
    REQUIRE(b.find(hay) == 16);

    a.swap(b);
    REQUIRE(a.valid());
    REQUIRE(b.valid());
    REQUIRE(a.find(hay) == 16);
    REQUIRE(b.find(hay) == 10);

    /* Valid <-> invalid. */
    memepp::searcher inv;
    a.swap(inv);
    REQUIRE(!a.valid());
    REQUIRE(inv.valid());
    REQUIRE(inv.find(hay) == 16);

    /* Invalid <-> invalid. */
    memepp::searcher i1, i2;
    i1.swap(i2);
    REQUIRE(!i1.valid());
    REQUIRE(!i2.valid());
}

TEST_CASE("memepp::searcher: self assignment", "[searcher]")
{
    memepp::string hay = "the quick brown fox";
    memepp::searcher s("brown");

    s = s;                       /* self copy-assign  */
    REQUIRE(s.valid());
    REQUIRE(s.find(hay) == 10);

    s = std::move(s);            /* self move-assign  */
    REQUIRE(s.valid());
    REQUIRE(s.find(hay) == 10);
}

TEST_CASE("memepp::searcher: move/copy from invalid", "[searcher]")
{
    memepp::string hay = "abc";

    memepp::searcher invalid;
    REQUIRE(!invalid.valid());

    /* Move construction from invalid. */
    memepp::searcher m(std::move(invalid));
    REQUIRE(!m.valid());
    REQUIRE(m.find(hay) == -1);

    /* Move assignment from invalid over a valid target. */
    memepp::searcher dst("abc");
    REQUIRE(dst.valid());
    dst = std::move(m);
    REQUIRE(!dst.valid());
    REQUIRE(dst.find(hay) == -1);

    /* Copy assignment from invalid over a valid target. */
    memepp::searcher dst2("abc");
    REQUIRE(dst2.valid());
    dst2 = invalid;
    REQUIRE(!dst2.valid());
    REQUIRE(dst2.find(hay) == -1);

    /* All of them must leave a zeroed handle. */
    const unsigned char* raw =
        reinterpret_cast<const unsigned char*>(dst2.native_handle());
    for (size_t i = 0; i < sizeof(MemeSearcherStack_t); ++i)
        REQUIRE(raw[i] == 0);
}

TEST_CASE("memepp::searcher: native_handle around", "[searcher]")
{
    memepp::searcher s("abc");
    memepp::searcher& mut = s;
    REQUIRE(mut.native_handle() != nullptr);
    REQUIRE(std::is_same<decltype(mut.native_handle()),
                         mmsrchstk_t*>::value);

    const memepp::searcher& c = s;
    REQUIRE(c.native_handle() != nullptr);
    REQUIRE(std::is_same<decltype(c.native_handle()),
                         const MemeSearcherStack_t*>::value);
}

TEST_CASE("memepp::searcher: density rare and sparse", "[searcher]")
{
    memepp::searcher s("fox");
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";

    /* 1/10 = 10 % -> rare */
    s.find(hit);
    for (int i = 0; i < 9; ++i) s.find(miss);
    REQUIRE(s.density() == memepp::search_density_t::rare);

    s.reset_density();

    /* 3/10 = 30 % -> sparse */
    for (int i = 0; i < 3; ++i) s.find(hit);
    for (int i = 0; i < 7; ++i) s.find(miss);
    REQUIRE(s.density() == memepp::search_density_t::sparse);

    s.reset_density();

    /* 0 % hits is still rare, not unknown. */
    for (int i = 0; i < 4; ++i) s.find(miss);
    REQUIRE(s.density() == memepp::search_density_t::rare);
}

TEST_CASE("memepp::searcher: density on invalid instance", "[searcher]")
{
    memepp::searcher inv;
    REQUIRE(inv.density() == memepp::search_density_t::unknown);
    inv.reset_density();               /* must not crash */
    REQUIRE(inv.density() == memepp::search_density_t::unknown);
}

TEST_CASE("memepp::searcher: case sensitivity is honoured", "[searcher]")
{
    const MemeByte_t* nl   = reinterpret_cast<const MemeByte_t*>("AbCdEf");
    const MemeByte_t* hit  = reinterpret_cast<const MemeByte_t*>("xxabcdefyy");
    const MemeByte_t* nope = reinterpret_cast<const MemeByte_t*>("xxABCDEFyy");
    const mmint_t      hn   = 10;

    memepp::searcher ci(nl, 6, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.find(hit, hn) == 2);
    REQUIRE(ci.find(nope, hn) == 2);

    memepp::searcher cs(nl, 6, memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.find(hit, hn) == -1);   /* case differs -> miss */
    REQUIRE(cs.find(nope, hn) == -1);  /* needle is "AbCdEf", hay is "ABCDEF" */

    /* all_sensitive behaves like case_sensitive here. */
    memepp::searcher as(nl, 6, memepp::case_sensitivity_t::all_sensitive);
    REQUIRE(as.find(hit, hn) == -1);
    REQUIRE(as.find(nope, hn) == -1);

    /* Exact-case haystack must hit in both sensitive modes. */
    const MemeByte_t* exact = reinterpret_cast<const MemeByte_t*>("xxAbCdEfyy");
    REQUIRE(cs.find(exact, hn) == 2);
    REQUIRE(as.find(exact, hn) == 2);
}

TEST_CASE("memepp::searcher: Boyer-Moore case folding", "[searcher]")
{
    /* Mixed-case long needle: folding must be applied on BOTH the needle and
     * the haystack, otherwise the tables and the comparison disagree. */
    std::string needle = "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789";
    std::string lowercase(needle);
    for (char& ch : lowercase)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

    std::string hay_lower = "xx" + lowercase + "yy";
    std::string hay_upper = "xx" + needle + "yy";
    std::string miss(needle.size(), 'q');

    memepp::searcher s(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());

    REQUIRE(s.find(reinterpret_cast<const MemeByte_t*>(hay_lower.data()),
                   static_cast<mmint_t>(hay_lower.size())) == 2);
    REQUIRE(s.find(reinterpret_cast<const MemeByte_t*>(hay_upper.data()),
                   static_cast<mmint_t>(hay_upper.size())) == 2);
    REQUIRE(s.find(reinterpret_cast<const MemeByte_t*>(miss.data()),
                   static_cast<mmint_t>(miss.size())) == -1);
}

/* --------------------------------------------------------------------- */
/* New interface coverage                                                 */
/* --------------------------------------------------------------------- */

TEST_CASE("memepp::searcher: npos", "[searcher]")
{
    REQUIRE(memepp::searcher::npos == static_cast<memepp::searcher::size_type>(-1));
}

TEST_CASE("memepp::searcher: state and error_code", "[searcher]")
{
    /* Default-constructed: empty, no error. */
    {
        memepp::searcher s;
        REQUIRE(!s.valid());
        REQUIRE(s.state() == memepp::searcher_state_t::empty);
        REQUIRE(s.error_code() == MGEC__OK);
    }

    /* Failed constructor (nullptr). */
    {
        memepp::searcher s(static_cast<const char*>(nullptr));
    REQUIRE(!s.valid());
        REQUIRE(s.state() == memepp::searcher_state_t::failed);
        REQUIRE(s.error_code() == MGEC__INVAL);
    }

    /* Failed constructor (empty string). */
    {
        memepp::searcher s("");
        REQUIRE(!s.valid());
        REQUIRE(s.state() == memepp::searcher_state_t::failed);
        REQUIRE(s.error_code() == MGEC__INVAL);
    }

    /* Failed constructor (negative length). */
    {
        memepp::searcher s(reinterpret_cast<const MemeByte_t*>("x"), -1);
        REQUIRE(!s.valid());
        REQUIRE(s.state() == memepp::searcher_state_t::failed);
        REQUIRE(s.error_code() == MGEC__INVAL);
    }

    /* Successful constructor: ready, no error. */
    {
        memepp::searcher s("fox");
        REQUIRE(s.valid());
        REQUIRE(s.state() == memepp::searcher_state_t::ready);
        REQUIRE(s.error_code() == MGEC__OK);
    }
}

TEST_CASE("memepp::searcher: reset()", "[searcher]")
{
    memepp::searcher s("fox");
    REQUIRE(s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::ready);

    s.reset();
    REQUIRE(!s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::empty);
    REQUIRE(s.error_code() == MGEC__OK);
    REQUIRE(s.find("the quick brown fox") == memepp::searcher::npos);

    /* Repeat reset is safe. */
    s.reset();
    REQUIRE(!s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::empty);

    /* Reset an already-empty searcher is safe. */
    memepp::searcher e;
    e.reset();
    REQUIRE(!e.valid());
}

TEST_CASE("memepp::searcher: operator bool", "[searcher]")
{
    memepp::searcher s("fox");
    REQUIRE(s);

    memepp::searcher e;
    REQUIRE(!e);

    memepp::searcher inv("");
    REQUIRE(!inv);
}

TEST_CASE("memepp::searcher: needle_size()", "[searcher]")
{
    memepp::searcher short_s("fox");
    REQUIRE(short_s.needle_size() == 3);

    memepp::searcher long_s(reinterpret_cast<const MemeByte_t*>("a"), 42);
    REQUIRE(long_s.needle_size() == 42);

    memepp::searcher e;
    REQUIRE(e.needle_size() == 0);
}

TEST_CASE("memepp::searcher: case_sensitivity()", "[searcher]")
{
    memepp::searcher s1("fox");
    REQUIRE(s1.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);

    memepp::searcher s2("fox", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s2.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
}

TEST_CASE("memepp::searcher: case_sensitivity() on an empty searcher",
          "[searcher]")
{
    /*
     * An empty searcher must report the same case sensitivity as a
     * searcher that was reset: both are "unconfigured".  initEmpty()
     * zeroes the object, but unInit()/reset() explicitly set
     * MemeFlag_AllSensitive, so initEmpty() has to agree or the answer
     * would depend on how the object was emptied.
     */
    memepp::searcher empty;
    REQUIRE(empty.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);

    memepp::searcher reset_one("FOX", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(reset_one.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
    reset_one.reset();
    REQUIRE(reset_one.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);

    REQUIRE(empty.case_sensitivity() == reset_one.case_sensitivity());
}

#if MG_LANG__CXX17_AVAIL
TEST_CASE("memepp::searcher: find(std::string_view)", "[searcher]")
{
    memepp::searcher s("brown");
    std::string_view hv = "the quick brown fox";

    REQUIRE(s.find(hv) == 10);
    REQUIRE(s.find(hv, 11) == memepp::searcher::npos);

    std::string_view empty;
    REQUIRE(s.find(empty) == memepp::searcher::npos);
}
#endif

TEST_CASE("memepp::searcher: find(string_span)", "[searcher]")
{
    memepp::searcher s("brown");
    memepp::string_span sp("the quick brown fox");

    REQUIRE(s.find(sp) == 10);
    REQUIRE(s.find(sp, 11) == memepp::searcher::npos);

    memepp::string_span empty;
    REQUIRE(s.find(empty) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: find(buffer_span)", "[searcher]")
{
    memepp::searcher s("brown");
    const char* text = "the quick brown fox";
    memepp::buffer_span bp(reinterpret_cast<const MemeByte_t*>(text), 19);

    REQUIRE(s.find(bp) == 10);
    REQUIRE(s.find(bp, 11) == memepp::searcher::npos);

    memepp::buffer_span empty;
    REQUIRE(s.find(empty) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: contains()", "[searcher]")
{
    memepp::searcher s("brown");
    const char* text = "the quick brown fox";

    REQUIRE(s.contains(text));
    REQUIRE(s.contains(memepp::string_view(text, 19)));
    REQUIRE(s.contains(memepp::string_span(text, 19)));
    REQUIRE(s.contains(memepp::buffer_span(reinterpret_cast<const MemeByte_t*>(text), 19)));

    REQUIRE(!s.contains("no match here"));
}

TEST_CASE("memepp::searcher: string_span constructor", "[searcher]")
{
    memepp::string_span needle("fox");
    memepp::searcher s(needle);
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 3);

    memepp::string_span empty;
    memepp::searcher e(empty);
    REQUIRE(!e.valid());
}

TEST_CASE("memepp::searcher: buffer_span constructor", "[searcher]")
{
    memepp::buffer_span needle(reinterpret_cast<const MemeByte_t*>("fox"), 3);
    memepp::searcher s(needle);
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 3);

    memepp::buffer_span empty;
    memepp::searcher e(empty);
    REQUIRE(!e.valid());
}

TEST_CASE("memepp::searcher: move/copy preserve cs", "[searcher]")
{
    memepp::searcher src("FOX", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(src.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);

    memepp::searcher moved(std::move(src));
    REQUIRE(moved.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(src.state() == memepp::searcher_state_t::empty);
}

TEST_CASE("memepp::searcher: find_all pattern via needle_size", "[searcher]")
{
    memepp::searcher s("ana");
    memepp::string text = "banana";

    std::vector<memepp::searcher::size_type> hits;
    auto pos = s.find(text);
    while (pos != memepp::searcher::npos)
    {
        hits.push_back(pos);
        pos = s.find(text, pos + s.needle_size());
    }

    REQUIRE(hits.size() == 1);
    REQUIRE(hits[0] == 1);
}

TEST_CASE("memepp::searcher: swap preserves all state", "[searcher]")
{
    memepp::string hay = "the quick brown fox";

    memepp::searcher a("brown", memepp::case_sensitivity_t::all_insensitive);
    memepp::searcher b("fox");

    a.swap(b);

    REQUIRE(a.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);
    REQUIRE(b.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(a.needle_size() == 3);
    REQUIRE(b.needle_size() == 5);
    REQUIRE(a.find(hay) == 16);
    REQUIRE(b.find(hay) == 10);

    /* Swap with invalid. */
    memepp::searcher e;
    b.swap(e);
    REQUIRE(!b.valid());
    REQUIRE(e.valid());
    REQUIRE(e.needle_size() == 5);
    REQUIRE(e.find(hay) == 10);
}

/* --------------------------------------------------------------------- */
/* Deep coverage for the interfaces added in this change                  */
/* --------------------------------------------------------------------- */

TEST_CASE("memepp::searcher: every find overload returns npos on miss",
          "[searcher]")
{
    memepp::searcher s("zzz");
    const char* raw = "the quick brown fox";
    const size_t n   = std::strlen(raw);

    REQUIRE(s.find(memepp::string("the quick brown fox")) == memepp::searcher::npos);
    REQUIRE(s.find(memepp::string_view(raw, n)) == memepp::searcher::npos);
    REQUIRE(s.find(raw) == memepp::searcher::npos);
    REQUIRE(s.find(reinterpret_cast<const MemeByte_t*>(raw), (mmint_t)n)
            == memepp::searcher::npos);
#if MG_LANG__CXX17_AVAIL
    REQUIRE(s.find(std::string_view(raw, n)) == memepp::searcher::npos);
#endif
    REQUIRE(s.find(memepp::string_span(raw, (mmint_t)n)) == memepp::searcher::npos);
    REQUIRE(s.find(memepp::buffer_span(
                reinterpret_cast<const MemeByte_t*>(raw), (mmint_t)n))
            == memepp::searcher::npos);

    /* npos is a real sentinel, not a coincidental -1 in one overload. */
    REQUIRE(memepp::searcher::npos != 0);
}

TEST_CASE("memepp::searcher: npos unrelated to offset semantics", "[searcher]")
{
    /* Forward search only: npos must never mean "search from the end". */
    memepp::searcher s("a");
    memepp::string hay = "abcabc";

    REQUIRE(s.find(hay, 0) == 0);
    REQUIRE(s.find(hay, 3) == 3);
    REQUIRE(s.find(hay, 4) == memepp::searcher::npos);
    REQUIRE(s.find(hay, memepp::searcher::npos) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: reset then rebuild", "[searcher]")
{
    memepp::searcher s("fox");
    memepp::string hay = "the quick brown fox";
    REQUIRE(s.find(hay) == 16);

    s.reset();
    REQUIRE(!s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::empty);
    REQUIRE(s.needle_size() == 0);
    REQUIRE(s.error_code() == MGEC__OK);
    REQUIRE(s.find(hay) == memepp::searcher::npos);

    /* Build a different searcher in the same storage. */
    memepp::searcher rebuilt("brown");
    s = std::move(rebuilt);
    REQUIRE(s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::ready);
    REQUIRE(s.needle_size() == 5);
    REQUIRE(s.find(hay) == 10);
}

TEST_CASE("memepp::searcher: reset keeps the object reusable many times",
          "[searcher]")
{
    memepp::string hay = "abcabc";
    memepp::searcher s("abc");

    for (int i = 0; i < 5; ++i)
    {
        s.reset();
        REQUIRE(s.state() == memepp::searcher_state_t::empty);

        memepp::searcher again("abc");
        s = std::move(again);
        REQUIRE(s.valid());
        REQUIRE(s.find(hay) == 0);
    }
}

TEST_CASE("memepp::searcher: needle_size survives move/copy/swap",
          "[searcher]")
{
    memepp::searcher a("brown");          /* 5 bytes */
    REQUIRE(a.needle_size() == 5);

    memepp::searcher moved(std::move(a));
    REQUIRE(moved.needle_size() == 5);
    REQUIRE(a.needle_size() == 0);        /* moved-from is empty */

    memepp::searcher copied(moved);
    REQUIRE(copied.needle_size() == 5);

    memepp::searcher b("fox");            /* 3 bytes */
    REQUIRE(b.needle_size() == 3);

    copied.swap(b);
    REQUIRE(copied.needle_size() == 3);
    REQUIRE(b.needle_size() == 5);
}

TEST_CASE("memepp::searcher: needle_size covers Boyer-Moore needles",
          "[searcher]")
{
    /* Long + case-insensitive selects the Boyer-Moore path. */
    std::string needle(200, 'a');
    memepp::searcher s(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);

    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == static_cast<mmint_t>(needle.size()));

    std::string hay = "xx" + needle + "yy";
    REQUIRE(s.find(reinterpret_cast<const MemeByte_t*>(hay.data()),
                   static_cast<mmint_t>(hay.size())) == 2);

    /* needle_size is unchanged by searching. */
    REQUIRE(s.needle_size() == static_cast<mmint_t>(needle.size()));
}

TEST_CASE("memepp::searcher: case_sensitivity after reset and move",
          "[searcher]")
{
    memepp::searcher s("FOX", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);

    memepp::searcher m(std::move(s));
    REQUIRE(m.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);

    memepp::searcher c(m);
    REQUIRE(c.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);

    /*
     * reset() clears the configuration back to the default.
     *
     * The C layer records MemeFlag_AllSensitive, whose value differs from
     * case_sensitivity_t::all_sensitive once promoted to mmint_t on some
     * platforms (0xFFFFFFFF unsigned vs -1 signed).  case_sensitivity() must
     * normalise both representations, so this holds on every platform.
     */
    c.reset();
    REQUIRE(c.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);

    /* Explicitly cover both underlying representations. */
    REQUIRE(static_cast<uint32_t>(memepp::case_sensitivity_t::all_sensitive) ==
            static_cast<uint32_t>(MemeFlag_AllSensitive));
}

TEST_CASE("memepp::searcher: contains() negative and invalid cases",
          "[searcher]")
{
    memepp::searcher s("brown");
    const char* raw = "the quick brown fox";
    const mmint_t n = static_cast<mmint_t>(std::strlen(raw));

    /* Present. */
    REQUIRE(s.contains(raw));
    REQUIRE(s.contains(memepp::string_view(raw, n)));
    REQUIRE(s.contains(memepp::string_span(raw, n)));
    REQUIRE(s.contains(memepp::buffer_span(
                reinterpret_cast<const MemeByte_t*>(raw), n)));

    /* Absent. */
    REQUIRE(!s.contains("nothing here"));
    REQUIRE(!s.contains(memepp::string_view("nothing here", 12)));
    REQUIRE(!s.contains(memepp::string_span("nothing here", 12)));
    REQUIRE(!s.contains(memepp::buffer_span(
                reinterpret_cast<const MemeByte_t*>("nothing here"), 12)));

    /* Invalid searcher never contains anything. */
    memepp::searcher inv;
    REQUIRE(!inv.contains(raw));
    REQUIRE(!inv.contains(""));
}

TEST_CASE("memepp::searcher: operator bool across lifecycle", "[searcher]")
{
    memepp::searcher s;
    REQUIRE(!static_cast<bool>(s));       /* empty      */

    memepp::searcher failed("");
    REQUIRE(!static_cast<bool>(failed));  /* failed     */

    memepp::searcher ready("fox");
    REQUIRE(static_cast<bool>(ready));    /* ready      */

    ready.reset();
    REQUIRE(!static_cast<bool>(ready));   /* back to empty */

    /* if (...) uses the same operator. */
    int hits = 0;
    if (ready) ++hits;
    REQUIRE(hits == 0);
}

TEST_CASE("memepp::searcher: find(std::string_view) offset and invalid",
          "[searcher]")
{
#if MG_LANG__CXX17_AVAIL
    memepp::searcher s("abc");
    std::string_view hay = "abcabc";

    REQUIRE(s.find(hay, 0) == 0);
    REQUIRE(s.find(hay, 1) == 3);
    REQUIRE(s.find(hay, 3) == 3);
    REQUIRE(s.find(hay, 4) == memepp::searcher::npos);
    REQUIRE(s.find(hay, 6) == memepp::searcher::npos);
    REQUIRE(s.find(hay, 99) == memepp::searcher::npos);
    REQUIRE(s.find(hay, -1) == memepp::searcher::npos);

    /* Invalid searcher. */
    memepp::searcher inv;
    REQUIRE(inv.find(hay) == memepp::searcher::npos);

    /* Empty text. */
    REQUIRE(s.find(std::string_view()) == memepp::searcher::npos);
#endif
}

TEST_CASE("memepp::searcher: find(string_span) offset, BM path and NUL",
          "[searcher]")
{
    /* FastScan path with offsets. */
    memepp::searcher fast("abc");
    memepp::string_span hay("abcabc");
    REQUIRE(fast.find(hay, 0) == 0);
    REQUIRE(fast.find(hay, 3) == 3);
    REQUIRE(fast.find(hay, 6) == memepp::searcher::npos);
    REQUIRE(fast.find(hay, 99) == memepp::searcher::npos);
    REQUIRE(fast.find(memepp::string_span()) == memepp::searcher::npos);

    /* Boyer-Moore path. */
    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";
    memepp::searcher bm(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);

    memepp::string_span bmhay(big.data(), static_cast<mmint_t>(big.size()));
    REQUIRE(bm.find(bmhay) == 2);
    REQUIRE(bm.find(bmhay, 3) == memepp::searcher::npos);

    /* Embedded NUL is just a byte when the length is explicit. */
    const char with_nul[] = { 'x', 0, 'a', 'b', 'c' };
    memepp::string_span nul_span(with_nul, 5);
    REQUIRE(fast.find(nul_span) == 2);
}

TEST_CASE("memepp::searcher: find(buffer_span) offset, BM path and NUL",
          "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::searcher fast("abc");
    memepp::buffer_span hay(B("abcabc"), 6);
    REQUIRE(fast.find(hay, 0) == 0);
    REQUIRE(fast.find(hay, 3) == 3);
    REQUIRE(fast.find(hay, 6) == memepp::searcher::npos);
    REQUIRE(fast.find(hay, 99) == memepp::searcher::npos);
    REQUIRE(fast.find(memepp::buffer_span()) == memepp::searcher::npos);

    /* Boyer-Moore path. */
    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";
    memepp::searcher bm(
        B(needle.data()), static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    memepp::buffer_span bmhay(B(big.data()), static_cast<mmint_t>(big.size()));
    REQUIRE(bm.find(bmhay) == 2);

    /* Embedded NUL. */
    const MemeByte_t wy[] = { 'x', 0, 'a', 'b', 'c' };
    REQUIRE(fast.find(memepp::buffer_span(wy, 5)) == 2);

    /* Invalid searcher. */
    memepp::searcher inv;
    REQUIRE(inv.find(hay) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: string_span constructor with case modes",
          "[searcher]")
{
    memepp::string_span upper("ABC");
    memepp::string hay = "xxabcxx";

    memepp::searcher ci(upper, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.valid());
    REQUIRE(ci.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.needle_size() == 3);
    REQUIRE(ci.find(hay) == 2);

    memepp::searcher cs(upper, memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.find(hay) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: buffer_span constructor with case modes",
          "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::buffer_span upper(B("ABC"), 3);
    memepp::string hay = "xxabcxx";

    memepp::searcher ci(upper, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.valid());
    REQUIRE(ci.needle_size() == 3);
    REQUIRE(ci.find(hay) == 2);

    memepp::searcher cs(upper, memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.find(hay) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: state after move and copy", "[searcher]")
{
    memepp::searcher ready("fox");
    REQUIRE(ready.state() == memepp::searcher_state_t::ready);

    memepp::searcher moved(std::move(ready));
    REQUIRE(moved.state() == memepp::searcher_state_t::ready);
    REQUIRE(ready.state() == memepp::searcher_state_t::empty);

    memepp::searcher copied(moved);
    REQUIRE(copied.state() == memepp::searcher_state_t::ready);

    /* A failed searcher stays failed through a copy. */
    memepp::searcher failed("");
    REQUIRE(failed.state() == memepp::searcher_state_t::failed);
    memepp::searcher failed_copy(failed);
    REQUIRE(failed_copy.state() == memepp::searcher_state_t::failed);

    /* Move-assigning a failed over a ready one.  error_code() reflects the
     * thread-local errc, so re-trigger the failure immediately before
     * checking it -- another searcher may have cleared it in between. */
    memepp::searcher dst("abc");
    REQUIRE(dst.state() == memepp::searcher_state_t::ready);

    memepp::searcher failed_src("");
    dst = std::move(failed_src);
    REQUIRE(dst.state() == memepp::searcher_state_t::failed);
    REQUIRE(dst.error_code() == MGEC__INVAL);

    /* error_code() reads this object's own state, so building another
     * searcher must not change it. */
    REQUIRE(dst.state() == memepp::searcher_state_t::failed);
    REQUIRE(memepp::searcher("ok").error_code() == MGEC__OK);
    REQUIRE(dst.error_code() == MGEC__INVAL);
}

TEST_CASE("memepp::searcher: error_code on invalid inputs", "[searcher]")
{
    /* nullptr C string. */
    REQUIRE(memepp::searcher(static_cast<const char*>(nullptr)).error_code()
            == MGEC__INVAL);
    /* nullptr raw bytes. */
    REQUIRE(memepp::searcher(static_cast<const MemeByte_t*>(nullptr), 3)
                .error_code() == MGEC__INVAL);
    /* Zero length. */
    REQUIRE(memepp::searcher(reinterpret_cast<const MemeByte_t*>("x"), 0)
                .error_code() == MGEC__INVAL);
    /* Negative length. */
    REQUIRE(memepp::searcher(reinterpret_cast<const MemeByte_t*>("x"), -5)
                .error_code() == MGEC__INVAL);
    /* Empty views. */
#if MG_LANG__CXX17_AVAIL
    REQUIRE(memepp::searcher(std::string_view()).error_code() == MGEC__INVAL);
#endif
    REQUIRE(memepp::searcher(memepp::string_view()).error_code() == MGEC__INVAL);
    REQUIRE(memepp::searcher(memepp::string_span()).error_code() == MGEC__INVAL);
    REQUIRE(memepp::searcher(memepp::buffer_span()).error_code() == MGEC__INVAL);

    /* A successful build clears it again. */
    REQUIRE(memepp::searcher("x").error_code() == MGEC__OK);
}

TEST_CASE("memepp::searcher: buffer constructor", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::buffer needle(B("brown"), 5);
    memepp::searcher s(needle);
    REQUIRE(s.valid());
    REQUIRE(s.state() == memepp::searcher_state_t::ready);
    REQUIRE(s.needle_size() == 5);
    REQUIRE(s.case_sensitivity() == memepp::case_sensitivity_t::all_sensitive);

    memepp::string hay = "the quick brown fox";
    REQUIRE(s.find(hay) == 10);

    /* Case-insensitive. */
    memepp::buffer upper(B("BROWN"), 5);
    memepp::searcher ci(upper, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.valid());
    REQUIRE(ci.case_sensitivity() == memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.find(hay) == 10);

    /* Case-sensitive must miss. */
    memepp::searcher cs(upper, memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.find(hay) == memepp::searcher::npos);

    /* Empty buffer is rejected. */
    memepp::buffer empty;
    memepp::searcher e(empty);
    REQUIRE(!e.valid());
    REQUIRE(e.state() == memepp::searcher_state_t::failed);
    REQUIRE(e.error_code() == MGEC__INVAL);
}

TEST_CASE("memepp::searcher: buffer_view constructor", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::buffer owner(B("brown"), 5);
    memepp::buffer_view bv(owner);

    memepp::searcher s(bv);
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 5);

    memepp::string hay = "the quick brown fox";
    REQUIRE(s.find(hay) == 10);

    /* Empty view is rejected. */
    memepp::buffer empty_owner;
    memepp::buffer_view empty_bv(empty_owner);
    memepp::searcher e(empty_bv);
    REQUIRE(!e.valid());
    REQUIRE(e.error_code() == MGEC__INVAL);
}

TEST_CASE("memepp::searcher: find(buffer)", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::searcher s("brown");
    memepp::buffer hay(B("the quick brown fox"), 19);

    REQUIRE(s.find(hay) == 10);
    REQUIRE(s.find(hay, 0) == 10);
    REQUIRE(s.find(hay, 11) == memepp::searcher::npos);
    REQUIRE(s.find(hay, 19) == memepp::searcher::npos);
    REQUIRE(s.find(hay, 99) == memepp::searcher::npos);
    REQUIRE(s.find(hay, -1) == memepp::searcher::npos);

    /* Empty haystack. */
    memepp::buffer empty;
    REQUIRE(s.find(empty) == memepp::searcher::npos);

    /* Invalid searcher. */
    memepp::searcher inv;
    REQUIRE(inv.find(hay) == memepp::searcher::npos);

    /* Embedded NUL is just a byte. */
    const MemeByte_t wy[] = { 'x', 0, 'a', 'b', 'c' };
    memepp::buffer nul_buf(wy, 5);
    memepp::searcher abc("abc");
    REQUIRE(abc.find(nul_buf) == 2);
}

TEST_CASE("memepp::searcher: find(buffer_view)", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::searcher s("brown");
    memepp::buffer owner(B("the quick brown fox"), 19);
    memepp::buffer_view bv(owner);

    REQUIRE(s.find(bv) == 10);
    REQUIRE(s.find(bv, 11) == memepp::searcher::npos);

    /* Empty view. */
    memepp::buffer empty_owner;
    memepp::buffer_view empty_bv(empty_owner);
    REQUIRE(s.find(empty_bv) == memepp::searcher::npos);

    /* Invalid searcher. */
    memepp::searcher inv;
    REQUIRE(inv.find(bv) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: find(buffer) on the Boyer-Moore path", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";

    memepp::searcher bm(
        B(needle.data()), static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(bm.valid());

    memepp::buffer hay(B(big.data()), static_cast<mmint_t>(big.size()));
    REQUIRE(bm.find(hay) == 2);
    REQUIRE(bm.find(hay, 3) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: contains(buffer) and contains(buffer_view)",
          "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    memepp::searcher s("brown");
    memepp::buffer hit(B("the quick brown fox"), 19);
    memepp::buffer_view hit_bv(hit);

    REQUIRE(s.contains(hit));
    REQUIRE(s.contains(hit_bv));

    memepp::buffer miss(B("nothing here"), 12);
    memepp::buffer_view miss_bv(miss);
    REQUIRE(!s.contains(miss));
    REQUIRE(!s.contains(miss_bv));

    memepp::searcher inv;
    REQUIRE(!inv.contains(hit));
    REQUIRE(!inv.contains(hit_bv));
}

TEST_CASE("memepp::searcher: buffer needle lifecycle", "[searcher]")
{
    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    /* The searcher copies the needle, so destroying the source buffer must
     * not affect it. */
    memepp::searcher s;
    memepp::string hay = "the quick brown fox";
    {
        memepp::buffer tmp(B("brown"), 5);
        memepp::searcher built(tmp);
        s = std::move(built);
    }
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 5);
    REQUIRE(s.find(hay) == 10);
}

/* --------------------------------------------------------------------- */
/* Behaviour flags: concurrent find()                                    */
/* --------------------------------------------------------------------- */

TEST_CASE("memepp::searcher: flags default to none", "[searcher]")
{
    memepp::searcher s("fox");
    REQUIRE(s.flags() == memepp::searcher_flag_t::none);

    /* An empty searcher reports none too. */
    memepp::searcher e;
    REQUIRE(e.flags() == memepp::searcher_flag_t::none);
    /* Setting flags on an empty searcher is rejected, silently harmless. */
    e.set_flags(memepp::searcher_flag_t::disable_density);
    REQUIRE(e.flags() == memepp::searcher_flag_t::none);
}

TEST_CASE("memepp::searcher: disable_density stops counter updates",
          "[searcher]")
{
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";

    /* Without the flag, four calls make density() meaningful. */
    {
        memepp::searcher s("fox");
        REQUIRE(s.flags() == memepp::searcher_flag_t::none);
        for (int i = 0; i < 4; ++i) s.find(hit);
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }

    /* With the flag, find() must not touch the counters at all. */
    {
        memepp::searcher s("fox");
        s.set_flags(memepp::searcher_flag_t::disable_density);
        REQUIRE(s.flags() == memepp::searcher_flag_t::disable_density);

        for (int i = 0; i < 100; ++i)
        {
            REQUIRE(s.find(hit) == 16);
            REQUIRE(s.find(miss) == memepp::searcher::npos);
        }

        /* Still unknown: nothing was counted. */
        REQUIRE(s.density() == memepp::search_density_t::unknown);
    }
}

TEST_CASE("memepp::searcher: disable_density keeps results correct",
          "[searcher]")
{
    memepp::searcher s("brown", memepp::case_sensitivity_t::all_insensitive);
    s.set_flags(memepp::searcher_flag_t::disable_density);

    memepp::string hay = "the quick BROWN fox";
    REQUIRE(s.find(hay) == 10);
    REQUIRE(s.find(hay, 11) == memepp::searcher::npos);
    REQUIRE(s.contains(hay));

    /* Boyer-Moore path as well. */
    std::string needle(200, 'a');
    std::string big = "xx" + needle + "yy";
    memepp::searcher bm(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    bm.set_flags(memepp::searcher_flag_t::disable_density);
    REQUIRE(bm.find(reinterpret_cast<const MemeByte_t*>(big.data()),
                    static_cast<mmint_t>(big.size())) == 2);
}

TEST_CASE("memepp::searcher: flag enum operators", "[searcher]")
{
    using memepp::searcher_flag_t;

    /* The bitwise operators come from megopp/util/flag_enum.h. */
    const searcher_flag_t both =
        searcher_flag_t::none | searcher_flag_t::disable_density;
    REQUIRE(both == searcher_flag_t::disable_density);

    REQUIRE((both & searcher_flag_t::disable_density)
            == searcher_flag_t::disable_density);
    REQUIRE((both & searcher_flag_t::none) == searcher_flag_t::none);

    /* Round-trip through the searcher. */
    memepp::searcher s("fox");
    s.set_flags(both);
    REQUIRE(s.flags() == searcher_flag_t::disable_density);

    /* mgpp::util helpers work on it too. */
    REQUIRE(mgpp::util::has_flag(s.flags(), searcher_flag_t::disable_density));
    REQUIRE(mgpp::util::has_any_flag(s.flags()));
    REQUIRE(mgpp::util::has_no_flag(searcher_flag_t::none));
    REQUIRE(mgpp::util::count_flags(s.flags()) == 1);
}

TEST_CASE("memepp::searcher: flags survive copy", "[searcher]")
{
    memepp::searcher s("fox");
    s.set_flags(memepp::searcher_flag_t::disable_density);

    memepp::searcher c(s);
    REQUIRE(c.flags() == memepp::searcher_flag_t::disable_density);

    /* Move carries them too. */
    memepp::searcher m(std::move(c));
    REQUIRE(m.flags() == memepp::searcher_flag_t::disable_density);

    /* reset() clears them (back to a default-constructed object). */
    m.reset();
    REQUIRE(m.flags() == memepp::searcher_flag_t::none);
}


/* --------------------------------------------------------------------- */
/* Gap coverage: NUL-mode BM, embedded NUL, CS BM, positions, errors,     */
/* resource reuse, swap state, density boundaries, flags filtering        */
/* --------------------------------------------------------------------- */

/*
 * The u64 bad-char table width (needle length > UINT32_MAX) is not covered:
 * allocating a 4 GB+ needle is impractical in a unit test, and the u64
 * kernel is the same code as u8/u16/u32 with a wider table type.  The
 * dispatch switch (case 8 / default) is NOT exercised by the NUL-mode path
 * below: a 200-byte needle uses b_width == 1 (u8).
 */

TEST_CASE("MemeSearcherStack: NUL-terminated Boyer-Moore", "[searcher]")
{
    /* Needle longer than the searcher threshold (8) selects Boyer-Moore.
     * find(const char*) passes -1, so the NUL-mode BM branch is taken. */
    std::string needle(200, 'a');
    std::string hay = "xx" + needle + "yy";
    std::string miss(200, 'b');

    MemeSearcherStack_t s;
    REQUIRE(MemeSearcherStack_init(
        &s, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<MemeInteger_t>(needle.size()),
        MemeFlag_AllInsensitive) == 0);

    /* Hit at position 2 through the NUL-terminated BM path. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>(hay.c_str()), -1, 0) == 2);

    /* Miss. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>(miss.c_str()), -1, 0) == -1);

    /* Offset past the hit. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>(hay.c_str()), -1, 3) == -1);

    /* Offset exactly at the hit. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>(hay.c_str()), -1, 2) == 2);

    /* Haystack shorter than the needle: the up-front NUL probe stops at the
     * terminator before any comparison. */
    REQUIRE(MemeSearcherStack_find(
        &s, reinterpret_cast<const MemeByte_t*>("xx"), -1, 0) == -1);

    MemeSearcherStack_unInit(&s);

    /* Case-insensitive NUL-mode BM: the haystack is folded per byte. */
    std::string mixed = "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789";
    std::string lower(mixed);
    for (char& ch : lower)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    std::string ci_hay = "xx" + lower + "yy";

    MemeSearcherStack_t ci;
    REQUIRE(MemeSearcherStack_init(
        &ci, MMSEARCHER__OBJ_SIZE,
        reinterpret_cast<const MemeByte_t*>(mixed.data()),
        static_cast<MemeInteger_t>(mixed.size()),
        MemeFlag_AllInsensitive) == 0);
    REQUIRE(MemeSearcherStack_find(
        &ci, reinterpret_cast<const MemeByte_t*>(ci_hay.c_str()), -1, 0) == 2);
    MemeSearcherStack_unInit(&ci);
}

TEST_CASE("memepp::searcher: embedded NUL needle", "[searcher]")
{
    /* A needle containing a NUL byte sets the internal HasNul marker, which
     * disables the strstr() shortcut in NUL mode. */
    const MemeByte_t n[] = { 'a', 0, 'b' };
    memepp::searcher s(n, 3);
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 3);

    /* Fixed-length mode: the NUL is just a byte. */
    const MemeByte_t hay[] = { 'x', 'a', 0, 'b' };
    REQUIRE(s.find(hay, 4) == 1);

    /* NUL mode: the haystack is a C string, so it cannot contain the
     * needle's embedded NUL.  The search must miss.  If the HasNul marker
     * were lost, the strstr() shortcut would treat the needle as "a" and
     * incorrectly report a hit at position 1. */
    REQUIRE(s.find("xa") == memepp::searcher::npos);

    /* setFlags(none) must not clear the internal HasNul marker. */
    s.set_flags(memepp::searcher_flag_t::none);
    REQUIRE(s.find("xa") == memepp::searcher::npos);

    /* setFlags(disable_density) must not clear it either. */
    s.set_flags(memepp::searcher_flag_t::disable_density);
    REQUIRE(s.find("xa") == memepp::searcher::npos);

    /* Long embedded-NUL needle on the Boyer-Moore path. */
    std::string long_n(200, 'a');
    long_n[100] = '\0';
    memepp::searcher ls(
        reinterpret_cast<const MemeByte_t*>(long_n.data()), 200);
    REQUIRE(ls.valid());

    std::string long_hay = "xx" + long_n + "yy";
    REQUIRE(ls.find(
        reinterpret_cast<const MemeByte_t*>(long_hay.data()),
        static_cast<mmint_t>(long_hay.size())) == 2);

    /* NUL mode with a long embedded-NUL needle: the haystack is shorter
     * than the needle, so the up-front NUL probe stops at the terminator
     * and the search misses. */
    std::string cstr = "xx" + std::string(100, 'a');
    REQUIRE(ls.find(cstr.c_str()) == memepp::searcher::npos);
}

TEST_CASE("memepp::searcher: case-sensitive Boyer-Moore", "[searcher]")
{
    /* Long mixed-case needle with case sensitivity: the needle is stored
     * unfolded and the haystack is not folded (bm_fold == 0). */
    const char* mixed = "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789";
    std::string lower(mixed);
    for (char& ch : lower)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

    std::string exact_hay = "xx" + std::string(mixed) + "yy";
    std::string lower_hay = "xx" + lower + "yy";

    memepp::searcher cs(
        reinterpret_cast<const MemeByte_t*>(mixed),
        static_cast<mmint_t>(std::strlen(mixed)),
        memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.valid());
    REQUIRE(cs.find(
        reinterpret_cast<const MemeByte_t*>(exact_hay.data()),
        static_cast<mmint_t>(exact_hay.size())) == 2);
    REQUIRE(cs.find(
        reinterpret_cast<const MemeByte_t*>(lower_hay.data()),
        static_cast<mmint_t>(lower_hay.size())) == memepp::searcher::npos);

    /* The same needle case-insensitive matches both. */
    memepp::searcher ci(
        reinterpret_cast<const MemeByte_t*>(mixed),
        static_cast<mmint_t>(std::strlen(mixed)),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.find(
        reinterpret_cast<const MemeByte_t*>(exact_hay.data()),
        static_cast<mmint_t>(exact_hay.size())) == 2);
    REQUIRE(ci.find(
        reinterpret_cast<const MemeByte_t*>(lower_hay.data()),
        static_cast<mmint_t>(lower_hay.size())) == 2);
}

TEST_CASE("memepp::searcher: Boyer-Moore hit positions", "[searcher]")
{
    std::string needle(200, 'a');
    memepp::searcher s(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());

    auto B = [](const std::string& p) {
        return reinterpret_cast<const MemeByte_t*>(p.data());
    };

    /* Hit at position 0. */
    std::string h0 = needle + "yy";
    REQUIRE(s.find(B(h0), static_cast<mmint_t>(h0.size())) == 0);

    /* Hit at the end. */
    std::string hend = "xx" + needle;
    REQUIRE(s.find(B(hend), static_cast<mmint_t>(hend.size())) == 2);

    /* Haystack exactly the needle length. */
    REQUIRE(s.find(B(needle), static_cast<mmint_t>(needle.size())) == 0);

    /* Offset skips the only hit. */
    std::string hmid = "xx" + needle + "yy";
    REQUIRE(s.find(B(hmid), static_cast<mmint_t>(hmid.size()), 3)
            == memepp::searcher::npos);

    /* Offset exactly at the hit. */
    REQUIRE(s.find(B(hmid), static_cast<mmint_t>(hmid.size()), 2) == 2);

    /* Overlapping pattern: the needle is "ab" repeated 100 times. */
    std::string rep(200, ' ');
    for (size_t i = 0; i < 200; ++i)
        rep[i] = (i % 2 == 0) ? 'a' : 'b';
    memepp::searcher rs(
        reinterpret_cast<const MemeByte_t*>(rep.data()), 200,
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(rs.valid());

    std::string rhit = rep + "ab";
    REQUIRE(rs.find(B(rhit), static_cast<mmint_t>(rhit.size())) == 0);

    std::string rhit1 = "x" + rep + "ab";
    REQUIRE(rs.find(B(rhit1), static_cast<mmint_t>(rhit1.size())) == 1);
}

TEST_CASE("MemeSearcherStack: C API error branches", "[searcher]")
{
    const size_t sz = MMSEARCHER__OBJ_SIZE;
    const MemeByte_t* one = reinterpret_cast<const MemeByte_t*>("x");

    /* initEmpty. */
    REQUIRE(MemeSearcherStack_initEmpty(nullptr, sz) == MGEC__INVAL);
    {
        MemeSearcherStack_t s;
        REQUIRE(MemeSearcherStack_initEmpty(&s, 0) == MGEC__OPNOTSUPP);
        REQUIRE(MemeSearcherStack_initEmpty(
            &s, 0xFF * sizeof(mmint_t) + 1) == MGEC__OPNOTSUPP);
        REQUIRE(MemeSearcherStack_initEmpty(&s, sz) == 0);
    }

    /* init. */
    REQUIRE(MemeSearcherStack_init(nullptr, sz, one, 1, MemeFlag_CaseSensitive)
            == MGEC__INVAL);
    {
        MemeSearcherStack_t s;
        REQUIRE(MemeSearcherStack_init(&s, 0, one, 1, MemeFlag_CaseSensitive)
                == MGEC__OPNOTSUPP);
        REQUIRE(MemeSearcherStack_init(
            &s, sz, nullptr, 3, MemeFlag_CaseSensitive) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_init(
            &s, sz, one, 0, MemeFlag_CaseSensitive) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_init(
            &s, sz, one, 1, MemeFlag_CaseSensitive) == 0);
        MemeSearcherStack_unInit(&s);
    }

    /* initByOther. */
    {
        MemeSearcherStack_t a, b;
        REQUIRE(MemeSearcherStack_init(
            &a, sz, one, 1, MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_initByOther(nullptr, sz, &a) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_initByOther(&b, sz, nullptr) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_initByOther(&b, 0, &a) == MGEC__OPNOTSUPP);
        REQUIRE(MemeSearcherStack_initByOther(&a, sz, &a) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_initByOther(&b, sz, &a) == 0);
        MemeSearcherStack_unInit(&a);
        MemeSearcherStack_unInit(&b);
    }

    /* move. */
    {
        MemeSearcherStack_t a, b;
        REQUIRE(MemeSearcherStack_init(
            &a, sz, one, 1, MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_move(nullptr, sz, &a) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_move(&b, sz, nullptr) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_move(&b, 0, &a) == MGEC__OPNOTSUPP);
        REQUIRE(MemeSearcherStack_move(&a, sz, &a) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_move(&b, sz, &a) == 0);
        MemeSearcherStack_unInit(&b);
    }

    /* unInit. */
    REQUIRE(MemeSearcherStack_unInit(nullptr) == MGEC__INVAL);

    /* swap.  b must be initialised first: swap() byte-exchanges the two
     * objects, so an uninitialised b would copy stack garbage into a,
     * and unInit(&a) would then free a garbage heap_block. */
    {
        MemeSearcherStack_t a, b;
        REQUIRE(MemeSearcherStack_init(
            &a, sz, one, 1, MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_initEmpty(&b, sz) == 0);
        REQUIRE(MemeSearcherStack_swap(nullptr, &a) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_swap(&a, nullptr) == MGEC__INVAL);
        REQUIRE(MemeSearcherStack_swap(&a, &a) == 0);
        REQUIRE(MemeSearcherStack_swap(&a, &b) == 0);
        /* After the swap a holds the empty object, b the valid one. */
        REQUIRE(MemeSearcherStack_state(&a) == MemeSearcherState_Empty);
        REQUIRE(MemeSearcherStack_state(&b) == MemeSearcherState_Ready);
        MemeSearcherStack_unInit(&a);
        MemeSearcherStack_unInit(&b);
    }

    /* find. */
    {
        MemeSearcherStack_t s;
        REQUIRE(MemeSearcherStack_init(
            &s, sz, one, 1, MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_find(nullptr, one, 1, 0) == -1);
        REQUIRE(MemeSearcherStack_find(&s, nullptr, 1, 0) == -1);
        MemeSearcherStack_unInit(&s);
    }

    /* resetDensity. */
    REQUIRE(MemeSearcherStack_resetDensity(nullptr) == MGEC__INVAL);
    {
        MemeSearcherStack_t z;
        std::memset(&z, 0, sizeof(z));
        REQUIRE(MemeSearcherStack_resetDensity(&z) == MGEC__INVAL);
    }

    /* setFlags. */
    REQUIRE(MemeSearcherStack_setFlags(nullptr, MemeSearcherFlag_None)
            == MGEC__INVAL);
    {
        MemeSearcherStack_t z;
        std::memset(&z, 0, sizeof(z));
        REQUIRE(MemeSearcherStack_setFlags(&z, MemeSearcherFlag_None)
                == MGEC__INVAL);
    }

    /* Introspection on null. */
    REQUIRE(MemeSearcherStack_density(nullptr) == MemeSearcherDensity_Unknown);
    REQUIRE(MemeSearcherStack_needleSize(nullptr) == 0);
    REQUIRE(MemeSearcherStack_caseSensitivity(nullptr) == MemeFlag_AllSensitive);
    REQUIRE(MemeSearcherStack_errorCode(nullptr) == MGEC__INVAL);
    REQUIRE(MemeSearcherStack_state(nullptr) == MemeSearcherState_Empty);

    /* flags() on null - this was missing from the original test. */
    REQUIRE(MemeSearcherStack_flags(nullptr) == MemeSearcherFlag_None);
}

TEST_CASE("MemeSearcherStack: re-init after unInit", "[searcher]")
{
    /*
     * The C API contract: a caller that re-initialises an object that still
     * owns a heap block must call unInit() first.  init() memsets the object
     * before allocating, so a stale heap_block pointer would be lost and the
     * block leaked.  The C++ wrapper always unInits before re-init (reset(),
     * move-assign, copy-assign), so the safe pattern is what the tests
     * exercise here.
     */
    const size_t sz = MMSEARCHER__OBJ_SIZE;

    /* BM needle (heap block) -> unInit -> re-init with a different needle. */
    {
        std::string n1(200, 'a');
        std::string n2(300, 'b');
        std::string hay2 = "zz" + n2 + "ww";

        MemeSearcherStack_t s;
        REQUIRE(MemeSearcherStack_init(
            &s, sz, reinterpret_cast<const MemeByte_t*>(n1.data()),
            static_cast<MemeInteger_t>(n1.size()),
            MemeFlag_AllInsensitive) == 0);
        REQUIRE(MemeSearcherStack_unInit(&s) == 0);

        REQUIRE(MemeSearcherStack_init(
            &s, sz, reinterpret_cast<const MemeByte_t*>(n2.data()),
            static_cast<MemeInteger_t>(n2.size()),
            MemeFlag_AllInsensitive) == 0);
        REQUIRE(MemeSearcherStack_find(
            &s, reinterpret_cast<const MemeByte_t*>(hay2.data()),
            static_cast<MemeInteger_t>(hay2.size()), 0) == 2);
        MemeSearcherStack_unInit(&s);
    }

    /* unInit -> initEmpty -> init. */
    {
        MemeSearcherStack_t s;
        REQUIRE(MemeSearcherStack_init(
            &s, sz, reinterpret_cast<const MemeByte_t*>("fox"), 3,
            MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_unInit(&s) == 0);
        REQUIRE(MemeSearcherStack_initEmpty(&s, sz) == 0);
        REQUIRE(MemeSearcherStack_state(&s) == MemeSearcherState_Empty);
        REQUIRE(MemeSearcherStack_init(
            &s, sz, reinterpret_cast<const MemeByte_t*>("bar"), 3,
            MemeFlag_CaseSensitive) == 0);
        REQUIRE(MemeSearcherStack_find(
            &s, reinterpret_cast<const MemeByte_t*>("xxbar"), 5, 0) == 2);
        MemeSearcherStack_unInit(&s);
    }
}

TEST_CASE("memepp::searcher: swap edge cases and state", "[searcher]")
{
    memepp::string hay = "the quick brown fox";

    /* Self-swap is a no-op. */
    {
        memepp::searcher s("fox");
        s.swap(s);
        REQUIRE(s.valid());
        REQUIRE(s.find(hay) == 16);
    }

    /* Boyer-Moore <-> FastScan. */
    {
        std::string needle(200, 'a');
        std::string big = "xx" + needle + "yy";
        memepp::searcher bm(
            reinterpret_cast<const MemeByte_t*>(needle.data()),
            static_cast<mmint_t>(needle.size()),
            memepp::case_sensitivity_t::all_insensitive);
        memepp::searcher fast("fox");

        bm.swap(fast);
        REQUIRE(bm.needle_size() == 3);
        REQUIRE(fast.needle_size() == 200);
        REQUIRE(bm.find(hay) == 16);
        REQUIRE(fast.find(
            reinterpret_cast<const MemeByte_t*>(big.data()),
            static_cast<mmint_t>(big.size())) == 2);
    }

    /* Failed <-> ready: state and error_code travel with the object. */
    {
        memepp::searcher failed("");
        REQUIRE(failed.state() == memepp::searcher_state_t::failed);
        memepp::searcher ready("fox");
        REQUIRE(ready.state() == memepp::searcher_state_t::ready);

        failed.swap(ready);
        REQUIRE(failed.valid());
        REQUIRE(failed.state() == memepp::searcher_state_t::ready);
        REQUIRE(failed.error_code() == MGEC__OK);
        REQUIRE(failed.find(hay) == 16);
        REQUIRE(!ready.valid());
        REQUIRE(ready.state() == memepp::searcher_state_t::failed);
        REQUIRE(ready.error_code() == MGEC__INVAL);
    }

    /* Density counters travel with the object. */
    {
        memepp::searcher a("fox");
        for (int i = 0; i < 4; ++i) a.find(hay);
        REQUIRE(a.density() == memepp::search_density_t::frequent);

        memepp::searcher b("bar");
        REQUIRE(b.density() == memepp::search_density_t::unknown);

        a.swap(b);
        REQUIRE(b.density() == memepp::search_density_t::frequent);
        REQUIRE(a.density() == memepp::search_density_t::unknown);
    }

    /* Behaviour flags travel with the object. */
    {
        memepp::searcher a("fox");
        a.set_flags(memepp::searcher_flag_t::disable_density);
        memepp::searcher b("bar");

        a.swap(b);
        REQUIRE(b.flags() == memepp::searcher_flag_t::disable_density);
        REQUIRE(a.flags() == memepp::searcher_flag_t::none);
    }
}

TEST_CASE("memepp::searcher: density exact boundaries", "[searcher]")
{
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";

    /* Exactly 25 % (1/4): h == ceil(c/4), so it is Sparse, not Rare. */
    {
        memepp::searcher s("fox");
        s.find(hit);
        for (int i = 0; i < 3; ++i) s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::sparse);
    }

    /* Exactly 50 % (2/4): Common, not Sparse. */
    {
        memepp::searcher s("fox");
        s.find(hit); s.find(hit);
        s.find(miss); s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::common);
    }

    /* Exactly 75 % (3/4): Frequent, not Common. */
    {
        memepp::searcher s("fox");
        s.find(hit); s.find(hit); s.find(hit);
        s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }

    /* 25 % at c = 8 (2/8): Sparse. */
    {
        memepp::searcher s("fox");
        s.find(hit); s.find(hit);
        for (int i = 0; i < 6; ++i) s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::sparse);
    }

    /* 50 % at c = 8 (4/8): Common. */
    {
        memepp::searcher s("fox");
        for (int i = 0; i < 4; ++i) s.find(hit);
        for (int i = 0; i < 4; ++i) s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::common);
    }

    /* 75 % at c = 8 (6/8): Frequent. */
    {
        memepp::searcher s("fox");
        for (int i = 0; i < 6; ++i) s.find(hit);
        for (int i = 0; i < 2; ++i) s.find(miss);
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }
}

TEST_CASE("memepp::searcher: density not counted on early return",
          "[searcher]")
{
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";

    /* A null haystack returns before the density counters are touched. */
    {
        memepp::searcher s("fox");
        for (int i = 0; i < 3; ++i) s.find(hit);
        for (int i = 0; i < 5; ++i)
            s.find(static_cast<const char*>(nullptr));
        s.find(hit);
        /* 4 counted calls, 4 hits -> Frequent.  If the null calls had been
         * counted, the ratio would be 4/9 (t50 = ceil(9/2) = 5) and the
         * answer would be Sparse. */
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }

    /* An invalid offset returns before the counters are touched. */
    {
        memepp::searcher s("fox");
        for (int i = 0; i < 3; ++i) s.find(hit);
        for (int i = 0; i < 5; ++i)
            s.find(miss, -1);
        s.find(hit);
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }

    /* Re-enabling density after disable_density counts only the new calls.
     * The 4 miss calls after re-enable must be counted (0/4 = Rare). */
    {
        memepp::searcher s("fox");
        s.set_flags(memepp::searcher_flag_t::disable_density);
        for (int i = 0; i < 5; ++i) { s.find(hit); s.find(miss); }
        REQUIRE(s.density() == memepp::search_density_t::unknown);

        s.set_flags(memepp::searcher_flag_t::none);
        for (int i = 0; i < 4; ++i) s.find(miss);
        /* Critical: verify the re-enabled calls are actually counted. */
        REQUIRE(s.density() == memepp::search_density_t::rare);

        s.reset_density();
        for (int i = 0; i < 4; ++i) s.find(hit);
        REQUIRE(s.density() == memepp::search_density_t::frequent);
    }
}

TEST_CASE("memepp::searcher: flags filtering and HasNul", "[searcher]")
{
    /* Unknown bits are filtered: only DisableDensity survives. */
    {
        memepp::searcher s("fox");
        s.set_flags(static_cast<memepp::searcher_flag_t>(0xFF));
        REQUIRE(s.flags() == memepp::searcher_flag_t::disable_density);

        /* The internal HasNul bit (0x80) is not a caller-settable flag. */
        s.set_flags(static_cast<memepp::searcher_flag_t>(0x80));
        REQUIRE(s.flags() == memepp::searcher_flag_t::none);
    }

    /* The internal HasNul marker survives setFlags() calls. */
    {
        const MemeByte_t n[] = { 'a', 0, 'b' };
        memepp::searcher s(n, 3);
        REQUIRE(s.find("xa") == memepp::searcher::npos);

        s.set_flags(memepp::searcher_flag_t::disable_density);
        REQUIRE(s.flags() == memepp::searcher_flag_t::disable_density);
        REQUIRE(s.find("xa") == memepp::searcher::npos);

        s.set_flags(memepp::searcher_flag_t::none);
        REQUIRE(s.flags() == memepp::searcher_flag_t::none);
        REQUIRE(s.find("xa") == memepp::searcher::npos);
    }

    /* setFlags on an uninitialised object is rejected by the C layer. */
    {
        memepp::searcher e;
        REQUIRE(MemeSearcherStack_setFlags(
            e.native_handle(), MemeSearcherFlag_DisableDensity)
            == MGEC__INVAL);
        REQUIRE(e.flags() == memepp::searcher_flag_t::none);
    }

    /* setFlags on a failed object is rejected too (reg_size_ is 0). */
    {
        memepp::searcher failed("");
        REQUIRE(failed.state() == memepp::searcher_state_t::failed);
        REQUIRE(MemeSearcherStack_setFlags(
            failed.native_handle(), MemeSearcherFlag_DisableDensity)
            == MGEC__INVAL);
    }
}

TEST_CASE("memepp::searcher: NUL-mode Boyer-Moore with u16/u32 tables",
          "[searcher]")
{
    /* NUL-mode dispatch uses a switch on b_width.  Needle lengths that
     * select u16 (256..65535) and u32 (65536..UINT32_MAX) need explicit
     * NUL-mode coverage.  CI forces Boyer-Moore regardless of length. */

    auto B = [](const char* p) {
        return reinterpret_cast<const MemeByte_t*>(p);
    };

    /* u16 table: needle length 256 bytes. */
    {
        std::string needle(256, 'a');
        std::string hay = "xx" + needle + "yy";
        memepp::searcher s(
            B(needle.data()), static_cast<mmint_t>(needle.size()),
            memepp::case_sensitivity_t::all_insensitive);
        REQUIRE(s.valid());
        /* Fixed-length find uses u16 dispatch. */
        REQUIRE(s.find(B(hay.data()), static_cast<mmint_t>(hay.size())) == 2);
        /* NUL-mode also uses u16 dispatch. */
        REQUIRE(s.find(hay.c_str()) == 2);
        REQUIRE(s.find("xa") == memepp::searcher::npos);
    }

    /* u32 table: needle length 65536 bytes. */
    {
        std::string needle(65536, 'b');
        std::string hay = "zz" + needle + "ww";
        memepp::searcher s(
            B(needle.data()), static_cast<mmint_t>(needle.size()),
            memepp::case_sensitivity_t::all_insensitive);
        REQUIRE(s.valid());
        REQUIRE(s.find(B(hay.data()), static_cast<mmint_t>(hay.size())) == 2);
        REQUIRE(s.find(hay.c_str()) == 2);
    }
}

#if MG_LANG__CXX17_AVAIL
TEST_CASE("memepp::searcher: std::string_view with CI and BM",
          "[searcher]")
{
    /* This combination was missing from the interface matrix. */
    std::string_view needle = "AbCdEfGhIjKlMnOpQrStUvWxYz0123456789";
    std::string lower(needle);
    for (char& ch : lower)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

    memepp::searcher s(needle, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());

    std::string hay = "xx" + lower + "yy";
    REQUIRE(s.find(std::string_view(hay)) == 2);
    REQUIRE(s.find(std::string_view("nomatch")) == memepp::searcher::npos);
}
#endif

TEST_CASE("memepp::searcher: buffer_view BM and embedded NUL",
          "[searcher]")
{
    /* buffer_view with a long needle selects BM; embedded NUL is just a byte
     * in fixed-length mode.  This fills a gap in the interface matrix. */
    auto B = [](const char* p) { return reinterpret_cast<const MemeByte_t*>(p); };

    std::string needle(300, 'a');
    needle[150] = '\0';
    std::string hay = "xx" + needle + "yy";

    memepp::buffer_view nv(B(needle.data()), static_cast<mmint_t>(needle.size()));
    memepp::searcher s(nv, memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());

    memepp::buffer_view hv(B(hay.data()), static_cast<mmint_t>(hay.size()));
    REQUIRE(s.find(hv) == 2);

    /* NUL-mode with embedded NUL needle: the haystack is NUL-free, so the
     * up-front probe stops early and the search misses. */
    std::string short_hay = "xx" + std::string(100, 'a');
    REQUIRE(s.find(short_hay.c_str()) == memepp::searcher::npos);
}

/* --------------------------------------------------------------------- */
/* API contract documentation                                            */
/* --------------------------------------------------------------------- */

/*
 * Resource-reuse contract for MemeSearcherStack_init/initEmpty/initByOther:
 *
 * These functions memset the destination object before any other work.
 * If the object already owns a heap_block (from a previous successful
 * init), that pointer is lost and the block is leaked.  The safe pattern
 * is: init -> unInit -> re-init.
 *
 * MemeSearcherStack_move() documents that _out must not already own
 * resources; the other init variants should carry the same note.
 *
 * The C++ wrapper (searcher.hpp) always calls unInit() before re-init
 * (reset(), move-assign, copy-assign), so it is safe by construction.
 */


TEST_CASE("memepp::searcher: contains(memepp::string) miss paths",
          "[searcher]")
{
    /* contains(const memepp::string&) was only exercised on the hit path. */
    memepp::searcher s("brown");
    memepp::string hit  = "the quick brown fox";
    memepp::string miss = "nothing here";
    memepp::string empty;

    REQUIRE(s.contains(hit));
    REQUIRE_FALSE(s.contains(miss));
    REQUIRE_FALSE(s.contains(empty));

    /* Invalid searcher never contains anything, for every overload. */
    memepp::searcher inv;
    REQUIRE_FALSE(inv.contains(hit));
    REQUIRE_FALSE(inv.contains(miss));
    REQUIRE_FALSE(inv.contains(empty));
}

TEST_CASE("memepp::searcher: buffer_view case-insensitive search",
          "[searcher]")
{
    /* buffer_view find() with all_insensitive was not covered; every other
     * view type had a CI case but buffer_view only had sensitive ones. */
    auto B = [](const char* p) { return reinterpret_cast<const MemeByte_t*>(p); };

    memepp::buffer owner(B("HeLLo WoRLd"), 11);
    memepp::buffer_view bv(owner);

    memepp::searcher ci("world", memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(ci.valid());
    REQUIRE(ci.find(bv) == 6);
    REQUIRE(ci.contains(bv));

    memepp::searcher cs("world", memepp::case_sensitivity_t::case_sensitive);
    REQUIRE(cs.valid());
    REQUIRE(cs.find(bv) == memepp::searcher::npos);
    REQUIRE_FALSE(cs.contains(bv));
}

TEST_CASE("memepp::searcher: every find overload on the BM path",
          "[searcher]")
{
    /* The BM path was covered through find(raw bytes), find(string),
     * find(string_span), find(buffer) and find(buffer_view), but not through
     * every overload.  A 40-byte mixed-case needle selects Boyer-Moore and
     * exercises each forwarding function with the same expectation. */
    std::string needle(40, 'a');
    std::string hay = "xx" + needle + "yy";
    const mmint_t hn = static_cast<mmint_t>(hay.size());

    memepp::searcher s(
        reinterpret_cast<const MemeByte_t*>(needle.data()),
        static_cast<mmint_t>(needle.size()),
        memepp::case_sensitivity_t::all_insensitive);
    REQUIRE(s.valid());
    REQUIRE(s.needle_size() == 40);

    auto B = [](const char* p) { return reinterpret_cast<const MemeByte_t*>(p); };

    /* memepp::string */
    REQUIRE(s.find(memepp::string(B(hay.data()), hn)) == 2);

    /* memepp::string_view */
    REQUIRE(s.find(memepp::string_view(B(hay.data()), hn)) == 2);

    /* raw bytes with explicit length */
    REQUIRE(s.find(B(hay.data()), hn) == 2);

#if MG_LANG__CXX17_AVAIL
    /* std::string_view */
    REQUIRE(s.find(std::string_view(hay)) == 2);
#endif

    /* string_span */
    REQUIRE(s.find(memepp::string_span(B(hay.data()), hn)) == 2);

    /* buffer_span */
    REQUIRE(s.find(memepp::buffer_span(B(hay.data()), hn)) == 2);

    /* memepp::buffer */
    REQUIRE(s.find(memepp::buffer(B(hay.data()), hn)) == 2);

    /* memepp::buffer_view */
    {
        memepp::buffer owner(B(hay.data()), hn);
        memepp::buffer_view bv(owner);
        REQUIRE(s.find(bv) == 2);
    }

    /* Null-terminated */
    REQUIRE(s.find(hay.c_str()) == 2);
}

TEST_CASE("MemeSearcherStack: object size bounds on init/initByOther/move",
          "[searcher]")
{
    /*
     * MEME_SRCH_MAX_OBJECT_SIZE is 0xFF * sizeof(mmint_t).  A size above it
     * would be truncated by the reg_size_ cast and leave the object looking
     * uninitialised.  initEmpty() was covered; init(), initByOther() and
     * move() only had the zero case.
     */
    const size_t too_big = (size_t)0xFF * sizeof(mmint_t) + 1;
    const MemeByte_t* one = reinterpret_cast<const MemeByte_t*>("x");
    MemeSearcherStack_t a, b;

    REQUIRE(MemeSearcherStack_init(
        &a, too_big, one, 1, MemeFlag_CaseSensitive) == MGEC__OPNOTSUPP);
    REQUIRE(MemeSearcherStack_init(
        &a, MMSEARCHER__OBJ_SIZE, one, 1, MemeFlag_CaseSensitive) == 0);

    REQUIRE(MemeSearcherStack_initByOther(&b, too_big, &a) == MGEC__OPNOTSUPP);
    REQUIRE(MemeSearcherStack_move(&b, too_big, &a) == MGEC__OPNOTSUPP);

    /* The source is untouched by the rejected calls. */
    REQUIRE(MemeSearcherStack_state(&a) == MemeSearcherState_Ready);
    REQUIRE(MemeSearcherStack_find(
        &a, one, 1, 0) == 0);

    MemeSearcherStack_unInit(&a);
}

TEST_CASE("memepp::searcher: unInit then reuse the same storage",
          "[searcher]")
{
    /*
     * The C++ wrapper owns the unInit/re-init sequencing: reset() releases
     * the heap block, and a later assignment rebuilds from scratch.  Cycling
     * several times checks that no stale pointer survives and that a BM
     * object (heap block) and a FastScan object (inline needle) can succeed
     * each other in the same storage.
     */
    memepp::string hay = "the quick brown fox";
    std::string big_needle(200, 'a');
    std::string big_hay = "xx" + big_needle + "yy";

    memepp::searcher s;
    for (int i = 0; i < 4; ++i)
    {
        /* FastScan */
        s = memepp::searcher("fox");
        REQUIRE(s.find(hay) == 16);

        s.reset();
        REQUIRE(!s.valid());
        REQUIRE(s.needle_size() == 0);

        /* Boyer-Moore, with a heap block */
        s = memepp::searcher(
            reinterpret_cast<const MemeByte_t*>(big_needle.data()),
            static_cast<mmint_t>(big_needle.size()),
            memepp::case_sensitivity_t::all_insensitive);
        REQUIRE(s.find(
            reinterpret_cast<const MemeByte_t*>(big_hay.data()),
            static_cast<mmint_t>(big_hay.size())) == 2);

        s.reset();
        REQUIRE(!s.valid());
    }

    /* The object is still usable after all that cycling. */
    s = memepp::searcher("brown");
    REQUIRE(s.find(hay) == 10);
}
