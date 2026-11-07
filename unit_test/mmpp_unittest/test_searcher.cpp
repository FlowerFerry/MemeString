#include <catch2/catch.hpp>

#include <memepp/searcher.hpp>
#include <memepp/string.hpp>
#include <cstring>
#include <string>

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

    /* Boyer-Moore case with b_width == 1: both the needle and the bad-char
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
     * case-sensitive flag forces fast-scan).
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

    memepp::searcher from_sv(std::string_view("abc"));
    REQUIRE(from_sv.valid());
    REQUIRE(from_sv.find(hay) == 0);

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
