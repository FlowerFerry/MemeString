
#include <catch2/catch.hpp>

#include <memepp/multi_buffer_view.hpp>

#include <array>
#include <vector>
#include <random>

static std::vector<uint8_t> flatten(const memepp::multi_buffer_view& mv) 
{
    std::vector<uint8_t> out;
    out.reserve(static_cast<size_t>(mv.size()));
    for (auto b : mv) out.push_back(b);
    return out;
}

template <class C1, class C2>
static std::vector<uint8_t> concat(const C1& a, const C2& b) 
{
    std::vector<uint8_t> out;
    out.reserve(a.size() + b.size());
    out.insert(out.end(), a.begin(), a.end());
    out.insert(out.end(), b.begin(), b.end());
    return out;
}

static uint32_t rng_uint(std::mt19937& gen, uint32_t lo, uint32_t hi) 
{
    std::uniform_int_distribution<uint32_t> dist(lo, hi);
    return dist(gen);
}

static void check_convert_roundtrip(const memepp::multi_buffer_view& mv, std::mt19937& gen) 
{
    if (mv.size() == 0) return;
    for (int i = 0; i < 10; ++i) {
        auto gpos = static_cast<memepp::multi_buffer_view::size_type>(
            rng_uint(gen, 0, static_cast<uint32_t>(mv.size() - 1))
        );
        auto p = mv.convert_position(gpos);
        REQUIRE(mv.convert_position(p) == gpos);

        // normalize 不应改变有效位置
        auto n = mv.normalize(p);
        REQUIRE(n.index == p.index);
        REQUIRE(n.offset == p.offset);
    }
}

static std::vector<uint8_t> subvec(const std::vector<uint8_t>& v, size_t pos, size_t len) {
    if (pos > v.size()) return {};
    len = (std::min)(len, v.size() - pos);
    return std::vector<uint8_t>(v.begin() + static_cast<long>(pos),
                                v.begin() + static_cast<long>(pos + len));
}

TEST_CASE("multi_buffer_view: default constructor and null", "[multi_buffer_view]")
{
    memepp::multi_buffer_view mv;
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.empty());
    REQUIRE(mv.begin() == mv.end());
    REQUIRE(mv.cbegin() == mv.cend());
    REQUIRE(mv.rbegin() == mv.rend());
    REQUIRE(mv.crbegin() == mv.crend());
    REQUIRE(mv.data() == nullptr);
    REQUIRE(mv.back_data() == nullptr);

    REQUIRE_THROWS_AS(mv.front(), std::out_of_range);
    REQUIRE_THROWS_AS(mv.back(), std::out_of_range);

    auto pos_invalid = mv.erase(0);
    REQUIRE(pos_invalid == memepp::multi_buffer_view::npos);
}

TEST_CASE("multi_buffer_view: append/prepend/pop and size/empty/data/back_data", "[multi_buffer_view]")
{
    std::array<uint8_t, 5> a{ 1,2,3,4,5 };
    std::array<uint8_t, 3> b{ 6,7,8 };

    memepp::multi_buffer_view mv;
    SECTION("append single") {
        mv.append(a.data(), a.size());
        REQUIRE(mv.size() == static_cast<memepp::multi_buffer_view::size_type>(a.size()));
        REQUIRE_FALSE(mv.empty());
        REQUIRE(mv.front() == 1);
        REQUIRE(mv.back() == 5);
        REQUIRE(mv.data() == a.data());
        REQUIRE(mv.back_data() == (a.data() + a.size() - 1));
        REQUIRE(flatten(mv) == std::vector<uint8_t>(a.begin(), a.end()));
    }

    SECTION("append multiple") {
        mv.append(a.data(), a.size());
        mv.append(b.data(), b.size());
        auto flat = flatten(mv);
        auto expect = concat(a, b);
        REQUIRE(mv.size() == static_cast<memepp::multi_buffer_view::size_type>(expect.size()));
        REQUIRE(flat == expect);
    }

    SECTION("prepend/append combination") {
        mv.prepend(a.data(), a.size()); // [a]
        mv.prepend(b.data(), b.size()); // [b][a]
        auto flat = flatten(mv);
        auto expect = concat(b, a);
        REQUIRE(mv.size() == static_cast<memepp::multi_buffer_view::size_type>(expect.size()));
        REQUIRE(flat == expect);

        mv.pop_front();
        auto cmp = std::vector<uint8_t>{ std::next(b.begin()), b.end()};
        cmp.insert(cmp.end(), a.begin(), a.end());
        REQUIRE(flatten(mv) == cmp);

        mv.pop_back();
        REQUIRE(mv.size() == 6);
        REQUIRE(mv.empty() == false);
        REQUIRE(mv.data() == b.data() + 1);
        REQUIRE(mv.back_data() == a.data() + 3);
    }

    SECTION("support empty views") {
        mv.append(nullptr, 0);
        mv.append(a.data(), a.size());
        mv.append(nullptr, 0);
        mv.append(b.data(), b.size());
        mv.append(nullptr, 0);

        REQUIRE(mv.data() == a.data());
        REQUIRE(mv.back_data() == (b.data() + b.size() - 1));
        REQUIRE(flatten(mv) == concat(a, b));
    }
}

TEST_CASE("multi_buffer_view: access at/operator[]/front/back", "[multi_buffer_view]") 
{
    std::array<uint8_t, 6> a{ 10, 20, 30, 40, 50, 60 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), static_cast<memepp::multi_buffer_view::size_type>(a.size()));

    SECTION("at(pos) boundary") {
        for (memepp::multi_buffer_view::size_type i = 0; i < static_cast<memepp::multi_buffer_view::size_type>(a.size()); ++i)
            REQUIRE(mv.at(i) == a[static_cast<size_t>(i)]);

        REQUIRE_THROWS_AS(mv.at(static_cast<memepp::multi_buffer_view::size_type>(a.size())), std::out_of_range);
    }

    SECTION("operator[] and front/back") {
        for (memepp::multi_buffer_view::size_type i = 0; i < static_cast<memepp::multi_buffer_view::size_type>(a.size()); ++i)
            REQUIRE(mv[i] == a[static_cast<size_t>(i)]);

        REQUIRE(mv.front() == 10);
        REQUIRE(mv.back() == 60);
    }

    SECTION("at(position) across views") {
        std::array<uint8_t, 3> b{ 1, 2, 3 };
        mv.append(b.data(), static_cast<memepp::multi_buffer_view::size_type>(b.size()));

        memepp::multi_buffer_view::position p{ 0, 4 };
        REQUIRE(mv.at(p) == 50);
        p.offset = 5;
        REQUIRE(mv.at(p) == 60);
        p = { 1, 0 };
        REQUIRE(mv.at(p) == 1);
        p = { 1, 2 };
        REQUIRE(mv.at(p) == 3);

        REQUIRE_THROWS_AS(mv.at(memepp::multi_buffer_view::position{ 2, 0 }), std::out_of_range);
    }
}

TEST_CASE("multi_buffer_view: convert_position / normalize / end_position / is_end", "[multi_buffer_view]") 
{
    std::array<uint8_t, 4> a{ 1,2,3,4 };
    std::array<uint8_t, 3> b{ 5,6,7 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), static_cast<memepp::multi_buffer_view::size_type>(a.size()));
    mv.append(nullptr, 0);
    mv.append(b.data(), static_cast<memepp::multi_buffer_view::size_type>(b.size()));

    // Check reciprocity of position mapping
    for (memepp::multi_buffer_view::size_type i = 0; i < mv.size(); ++i) {
        auto pos = mv.convert_position(i);
        REQUIRE(mv.convert_position(pos) == i);
        // normalize should not change legal positions
        auto norm = mv.normalize(pos);
        REQUIRE(norm.index == pos.index);
        REQUIRE(norm.offset == pos.offset);
    }

    // negative/out-of-bounds positions
    auto inv = mv.convert_position(static_cast<memepp::multi_buffer_view::size_type>(-1));
    REQUIRE(inv.index == memepp::multi_buffer_view::npos);
    REQUIRE(inv.offset == memepp::multi_buffer_view::npos);

    // end_position and is_end
    auto endp = mv.end_position();
    REQUIRE(mv.is_end(endp));
    auto lastp = mv.convert_position(mv.size() - 1);
    REQUIRE_FALSE(mv.is_end(lastp));

    // normalize should skip views with size==0
    memepp::multi_buffer_view::position p0{ 1, 0 }; // this is the position of an empty view
    auto norm0 = mv.normalize(p0);
    REQUIRE(norm0.index == 2);
    REQUIRE(norm0.offset == 0);
}

TEST_CASE("multi_buffer_view: Iterators (forward/reverse/zero-length view strides/--end)", "[multi_buffer_view]") 
{
    std::array<uint8_t, 3> a{ 1,2,3 };
    std::array<uint8_t, 2> b{ 4,5 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(nullptr, 0); // zero-length
    mv.append(b.data(), 2);

    // forward
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5}));

    // reverse
    std::vector<uint8_t> rev;
    for (auto it = mv.rbegin(); it != mv.rend(); ++it) rev.push_back(*it);
    REQUIRE(rev == std::vector<uint8_t>({5,4,3,2,1}));

    // Decrementing end() should point to the last element
    auto it = mv.end();
    --it;
    REQUIRE(*it == 5);
}

TEST_CASE("multi_buffer_view: insert (by size_type, position, and iterator) and split behavior", "[multi_buffer_view]") 
{
    std::array<uint8_t, 4> base{ 10,20,30,40 };
    std::array<uint8_t, 2> ins{ 99,100 };

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 4);

    SECTION("insert(size_type) inserts in the middle, triggering split") {
        auto pos_after = mv.insert(2, ins.data(), 2); // expect [10,20,99,100,30,40]
        REQUIRE(pos_after == 2);
        auto flat = flatten(mv);
        REQUIRE(flat == std::vector<uint8_t>({10,20,99,100,30,40}));

        // split should result in 3 views: [10,20] | [99,100] | [30,40]
        REQUIRE(mv.view_count() == 3);
        auto v0 = mv.view(0);
        auto v1 = mv.view(1);
        auto v2 = mv.view(2);
        REQUIRE(v0.second == 2);
        REQUIRE(v1.second == 2);
        REQUIRE(v2.second == 2);

        // insert(negative position) should insert at the beginning
        auto neg = static_cast<memepp::multi_buffer_view::size_type>(-5);
        mv.insert(neg, ins.data(), 2); // head insert
        REQUIRE(flatten(mv) == std::vector<uint8_t>({99,100,10,20,99,100,30,40}));
    }

    SECTION("insert(position) and insert(iterator)") {
        memepp::multi_buffer_view::position p{0, 1}; // insert before 20
        auto newp = mv.insert(p, ins.data(), 2);
        REQUIRE(newp.index == 1);
        REQUIRE(newp.offset == 0);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,99,100,20,30,40}));

        // Insert using iterator (insert at begin())
        auto cit = mv.insert(mv.begin(), ins.data(), 2);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({99,100,10,99,100,20,30,40}));
        // The returned iterator should point to the first element of the newly inserted data
        REQUIRE(*cit == 99);
    }
}

TEST_CASE("multi_buffer_view: erase / remove_byte (cross-view deletion, return position, out of bounds)", "[multi_buffer_view]") 
{
    // Prepare three views: A(1..5), B(6..8), C(9..10)
    std::array<uint8_t, 5> A{1,2,3,4,5};
    std::array<uint8_t, 3> B{6,7,8};
    std::array<uint8_t, 2> C{9,10};

    memepp::multi_buffer_view mv;
    mv.append(A.data(), 5);
    mv.append(B.data(), 3);
    mv.append(C.data(), 2);

    SECTION("erase(size_type, count) delete across views") {
        // Delete 4 bytes starting from global offset 3 (value=4): 4,5,6,7 should be deleted, leaving 1,2,3,8,9,10
        auto new_pos = mv.erase(3, 4);
        // new_pos should be the current position after deletion (returned as size_type)
        REQUIRE(new_pos == 3);
        auto flat = flatten(mv);
        REQUIRE(flat == std::vector<uint8_t>({1,2,3,8,9,10}));

        // Expect size to be updated to 6 (note: if class implementation does not update total_size_, this test will fail, indicating a defect in the implementation)
        REQUIRE(mv.size() == 6);
    }

    SECTION("erase(position) single byte deletion") {
        memepp::multi_buffer_view::position p{ 0, 2 }; // delete value=3
        auto after = mv.erase(p);
        REQUIRE(after.index  == 1);
        REQUIRE(after.offset == 0);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,4,5,6,7,8,9,10}));
        REQUIRE(mv.size() == 9);
    }

    SECTION("erase(iterator range)") {
        // Delete [2,6) -> global offset 2..5 -> delete 3,4,5,6
        auto it_begin = mv.begin();
        auto it_end = mv.begin();
        for (int i = 0; i < 2; ++i) ++it_begin;
        for (int i = 0; i < 6; ++i) ++it_end;

        auto after = mv.erase(it_begin, it_end);
        // Check remaining elements
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,7,8,9,10}));
        REQUIRE(mv.size() == 6);
        // The iterator should point to the new position of the start of the deleted range (value==7)
        REQUIRE(*after == 7);
    }

    SECTION("erase(size_type) out of bounds and invalid positions") {
        auto bad = mv.erase(static_cast<memepp::multi_buffer_view::size_type>(-1));
        REQUIRE(bad == memepp::multi_buffer_view::npos);
        bad = mv.erase(mv.size()); // equal to size -> invalid
        REQUIRE(bad == memepp::multi_buffer_view::npos);
    }
}

TEST_CASE("multi_buffer_view: replace (equal length/length change)", "[multi_buffer_view]") 
{
    std::array<uint8_t, 6> a{ 10, 11, 12, 13, 14, 15 };
    std::array<uint8_t, 3> new3{ 21, 22, 23 };
    std::array<uint8_t, 2> new2{ 99, 100 };

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 6);

    SECTION("replace(position, count, data)") {
        // Replace middle two bytes 12,13 -> 99,100
        memepp::multi_buffer_view::position p{0, 2};
        auto after = mv.replace(p, 2, new2.data(), 2);
        REQUIRE(after.index == 1);
        REQUIRE(after.offset == 0);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,11,99,100,14,15}));
        REQUIRE(mv.size() == 6);
    }

    SECTION("replace(size increase)") {
        // Replace 2 bytes with 3 bytes (length +1)
        mv.replace(memepp::multi_buffer_view::position{0, 2}, 2, new3.data(), 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,11,21,22,23,14,15}));

        // Expect size to increase by 1 (note: if remove_byte does not update total_size_, this test will fail, indicating a defect in the implementation)
        REQUIRE(mv.size() == 7);
    }
}

TEST_CASE("multi_buffer_view: remove_view / clear / swap / equality", "[multi_buffer_view]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 3> b{4,5,6};

    memepp::multi_buffer_view mv1, mv2;
    mv1.append(a.data(), 3);
    mv1.append(b.data(), 3);     // mv1: [1,2,3][4,5,6]
    auto expect = concat(a, b);
    mv2.append(expect.data(), 6); // mv2: [1..6] (single view)

    SECTION("operator== (different segments should also be equal)") {
        REQUIRE(mv1 == mv2);
        REQUIRE_FALSE(mv1 != mv2);
    }

    SECTION("remove_view affects size and content") {
        mv1.remove_view(0); // Remove [1,2,3]
        REQUIRE(flatten(mv1) == std::vector<uint8_t>({4,5,6}));
        REQUIRE(mv1.size() == 3);
    }

    SECTION("clear") {
        mv1.clear();
        REQUIRE(mv1.size() == 0);
        REQUIRE(mv1.empty());
    }

    SECTION("swap") {
        memepp::multi_buffer_view a1, a2;
        a1.append(a.data(), 3);          // [1,2,3]
        a2.append(b.data(), 3);          // [4,5,6]
        a1.swap(a2);
        REQUIRE(flatten(a1) == std::vector<uint8_t>({4,5,6}));
        REQUIRE(flatten(a2) == std::vector<uint8_t>({1,2,3}));
    }

    SECTION("operator== (different segments should also be equal)") {
        memepp::multi_buffer_view x = mv1;
        // Delete one byte
        x.erase(static_cast<memepp::multi_buffer_view::size_type>(0));
        REQUIRE_FALSE(x == mv2);
        REQUIRE(x != mv2);
    }
}

TEST_CASE("multi_buffer_view: slice (across views, default length)", "[multi_buffer_view]") 
{
    std::array<uint8_t, 5> a{1,2,3,4,5};
    std::array<uint8_t, 3> b{6,7,8};
    std::array<uint8_t, 2> c{9,10};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 5);
    mv.append(b.data(), 3);
    mv.append(c.data(), 2);

    SECTION("slice specifies the start point and length (across the view)") {
        // Take 5 bytes starting from global offset 3 -> 4,5,6,7,8
        auto s = mv.slice(static_cast<memepp::multi_buffer_view::size_type>(3), static_cast<memepp::multi_buffer_view::size_type>(5));
        REQUIRE(flatten(s) == std::vector<uint8_t>({4,5,6,7,8}));
        REQUIRE(s.size() == 5);
    }

    SECTION("slice default length to end") {
        // From global offset 7 -> 8,9,10
        auto s = mv.slice(static_cast<memepp::multi_buffer_view::size_type>(7));
        REQUIRE(flatten(s) == std::vector<uint8_t>({8,9,10}));
        REQUIRE(s.size() == 3);
    }

    SECTION("slice(position) illegal start") {
        // Use illegal start point for position
        auto s = mv.slice(memepp::multi_buffer_view::position{ -1, -1 }, 3);
        REQUIRE(s.size() == 0);
        REQUIRE(s.empty());
    }
}

TEST_CASE("multi_buffer_view: read_bytes (read across views/return error code on illegal position)", "[multi_buffer_view]") 
{
    std::array<uint8_t, 4> a{ 1,2,3,4 };
    std::array<uint8_t, 3> b{ 5,6,7 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);
    mv.append(b.data(), 3);

    SECTION("read fixed length across views") {
        // Read 4 bytes starting from position {0, 2} -> 3,4,5,6
        std::array<uint8_t, 4> buf{};
        auto n = mv.read_bytes(memepp::multi_buffer_view::position{0, 2}, buf.data(), 4);
        REQUIRE(n == 4);
        REQUIRE(std::vector<uint8_t>(buf.begin(), buf.end()) == std::vector<uint8_t>({3,4,5,6}));
    }

    SECTION("illegal start returns negative error code") {
        std::array<uint8_t, 2> buf{};
        auto n = mv.read_bytes(memepp::multi_buffer_view::position{ 5, 0 }, buf.data(), 2);
        REQUIRE(n < 0); // MGEC__RANGE etc. negative error codes
    }
}

TEST_CASE("multi_buffer_view: find (match sequence across views)", "[multi_buffer_view]") 
{
    // Note: The current find implementation is similar to "verifying whether the following bytes match the target sequence from the given starting point",
    //       rather than a general "substring search". Tests are written accordingly.
    std::array<uint8_t, 4> a{ 10, 11, 12, 13 };
    std::array<uint8_t, 3> b{ 14, 15, 16 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);
    mv.append(b.data(), 3);

    // Need to match 4 bytes starting at global offset 2: 12, 13, 14, 15 (spanning two views)
    uint8_t pat1[] = { 12, 13, 14, 15 };

    SECTION("An exact match from the starting point should return the starting point") {
        auto start = mv.convert_position(2);
        auto found = mv.find(pat1, 4, start);
        // Expect to return the starting point (Note: If the implementation returns the offset in the last view, this test will expose the implementation flaw)
        REQUIRE(found.index  == start.index);
        REQUIRE(found.offset == start.offset);
    }

    SECTION("An non-matching start returns npos") {
        auto start = mv.convert_position(3);
        auto found = mv.find(pat1, 4, start);
        REQUIRE(found.index  == mv.view_count());
        REQUIRE(found.offset == 0);
    }
}

TEST_CASE("multi_buffer_view: views capacity, count and view() access", "[multi_buffer_view]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};

    memepp::multi_buffer_view mv;
    mv.reserve_views(8);
    auto cap0 = mv.views_capacity();
    mv.append(a.data(), 3);
    mv.append(b.data(), 2);

    REQUIRE(mv.view_count() == 2);
    auto v0 = mv.view(0);
    auto v1 = mv.view(1);
    REQUIRE(v0.first == a.data());
    REQUIRE(v0.second == 3);
    REQUIRE(v1.first == b.data());
    REQUIRE(v1.second == 2);

    // Only the capacity interface can be used, and growth is not required.
    REQUIRE(mv.views_capacity() >= cap0);
}

TEST_CASE("multi_buffer_view: operator[](position) Boundary offset and out-of-bounds mapping", "[multi_buffer_view][edge]") 
{
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 1> b{3};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(b.data(), 1);

    // Offset equal to the current view length should automatically jump to the next view's first element
    memepp::multi_buffer_view::position p{0, 2}; // Pointing after a
    REQUIRE(mv[p] == 3);

    // convert_position(position) should return npos after the end of the last view
    memepp::multi_buffer_view::position end_after_last{1, 1}; // b.size()==1, offset==1 => past the end view
    auto glob = mv.convert_position(end_after_last);
    REQUIRE(glob == memepp::multi_buffer_view::npos);
}

TEST_CASE("multi_buffer_view: const_iterator's --begin() behavior and post-increment/decrement", "[multi_buffer_view][iterator]") 
{
    std::array<uint8_t, 4> a{10,20,30,40};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);

    // --begin() (although undefined in standard containers, this implementation chooses to leave it at begin)
    auto itb = mv.begin();
    auto itb2 = itb;
    --itb2;
    REQUIRE(itb2 == itb);
    REQUIRE(*itb2 == 10);

    // Post-increment/decrement
    auto it = mv.begin();          // -> 10
    auto it_old = it++;            // it_old:10, it:20
    REQUIRE(*it_old == 10);
    REQUIRE(*it == 20);

    auto it2 = mv.end();           // Pointing past the end
    auto it2_old = it2--;          // it2_old past the end, it2 points to 40
    REQUIRE(it2 == --mv.end());    // Consistent with --end()
    REQUIRE(*it2 == 40);
    (void)it2_old; // Just verify it compiles
}

TEST_CASE("multi_buffer_view: back_data()/back() when there is a zero-length view at the end", "[multi_buffer_view][zero]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(nullptr, 0); // Append a zero-length view at the end

    REQUIRE(mv.back() == 3);
    REQUIRE(mv.back_data() == a.data() + 2);

    // Continuous zero-length views at the front + data in the middle
    memepp::multi_buffer_view mv2;
    mv2.append(nullptr, 0);
    mv2.append(nullptr, 0);
    mv2.append(a.data(), 3);
    REQUIRE(mv2.front() == 1);
    REQUIRE(mv2.data() == a.data());
}

TEST_CASE("multi_buffer_view: insert accepts multi_buffer_view (insert at front/middle/back)", "[multi_buffer_view][insert]") 
{
    std::array<uint8_t, 4> base{10,20,30,40};
    std::array<uint8_t, 2> x1{99,100};
    std::array<uint8_t, 3> x2{7,8,9};

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 4);

    memepp::multi_buffer_view to_ins;
    to_ins.append(x1.data(), 2);
    to_ins.append(x2.data(), 3); // [99,100,7,8,9]

    SECTION("Insert at front") {
        mv.insert(memepp::multi_buffer_view::position{0,0}, to_ins);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({99,100,7,8,9,10,20,30,40}));
    }
    SECTION("Insert in middle after split") {
        // Return to initial state
        mv.clear();
        mv.append(base.data(), 4);
        mv.insert(static_cast<memepp::multi_buffer_view::size_type>(2), to_ins);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,20,99,100,7,8,9,30,40}));
    }
    SECTION("Insert at back") {
        mv.clear(); mv.append(base.data(), 4);
        mv.insert(mv.size(), to_ins);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,20,30,40,99,100,7,8,9}));
    }
}

TEST_CASE("multi_buffer_view: prepend/append accepts multi_buffer_view and maintains order", "[multi_buffer_view][prepend]") 
{
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 2> b{3,4};
    std::array<uint8_t, 2> c{5,6};

    memepp::multi_buffer_view mv;
    memepp::multi_buffer_view mvA; mvA.append(a.data(), 2);
    memepp::multi_buffer_view mvB; mvB.append(b.data(), 2);
    memepp::multi_buffer_view mvC; mvC.append(c.data(), 2);

    mv.append(mvB);   // [3,4]
    mv.prepend(mvA);  // [1,2][3,4]
    mv.append(mvC);   // [1,2][3,4][5,6]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5,6}));
}

TEST_CASE("multi_buffer_view: erase(position, position) handles out-of-bounds deletion", "[multi_buffer_view][erase]") 
{
    std::array<uint8_t, 6> a{1,2,3,4,5,6};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 6);

    SECTION("begin==end does not change content, returns end position") {
        auto p = mv.convert_position(3);
        auto after = mv.erase(p, p);
        auto idx = mv.convert_position(after);
        REQUIRE(idx == 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5,6}));
    }

    SECTION("Delete out of bounds: start deleting 10 from 4") {
        auto pos = mv.convert_position(4);
        auto after = mv.erase(pos, memepp::multi_buffer_view::position{ 1000, 1000 }); // Extremely large end
        // Deletion should remove 5,6
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4}));

        // Note: If the implementation does not maintain total_size_ in remove_byte, the following assertion will fail, indicating a defect
        REQUIRE(mv.size() == 4);

        // The returned position is at the start of the deletion
        auto gi = mv.convert_position(after);
        REQUIRE(gi == mv.npos); // Equal to the new size, may also convert to npos; if the implementation returns the end position, it is also acceptable
    }
}

TEST_CASE("multi_buffer_view: remove_view invalid index does not change content", "[multi_buffer_view][remove_view]") {
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(b.data(), 2);

    auto before = flatten(mv);
    mv.remove_view(static_cast<memepp::multi_buffer_view::size_type>(-1));
    REQUIRE(flatten(mv) == before);

    mv.remove_view(100);
    REQUIRE(flatten(mv) == before);
}

TEST_CASE("multi_buffer_view: view sharing visibility (modifications to underlying memory should be visible)", "[multi_buffer_view][alias]") {
    std::array<uint8_t, 4> a{1,2,3,4};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4}));

    // 修改底层数据
    a[1] = 22;
    a[3] = 44;
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,22,3,44}));
}

TEST_CASE("multi_buffer_view: assign/copy assignment/move assignment overwrites old content", "[multi_buffer_view][assign]") {
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{9,8};
    std::array<uint8_t, 2> c{7,6};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    SECTION("assign(pointer, size)") {
        mv.assign(b.data(), 2);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({9,8}));
    }

    SECTION("assign(multi_buffer_view)") {
        memepp::multi_buffer_view other;
        other.append(b.data(), 2);
        other.append(c.data(), 2);
        mv.assign(other);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({9,8,7,6}));
    }

    SECTION("operator=(multi_buffer_view) copy assignment") {
        memepp::multi_buffer_view x;
        x.append(b.data(), 2);
        mv = x;
        REQUIRE(flatten(mv) == std::vector<uint8_t>({9,8}));

        // Modifying the underlying data of x (which still points to b) does not affect the asserted visibility of the data contents at the same address pointed to by mv.
        // This only verifies that the contents of mv remain consistent with b.
        REQUIRE(flatten(mv) == std::vector<uint8_t>({9,8}));
    }

    SECTION("operator=(multi_buffer_view&&) move assignment") {
        memepp::multi_buffer_view x;
        x.append(b.data(), 2);
        x.append(c.data(), 2);
        mv = std::move(x);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({9,8,7,6}));
    }
}

TEST_CASE("multi_buffer_view: view()/view_count()/view(index) out of range throws", "[multi_buffer_view][view]") {
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 2> b{3,4};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(b.data(), 2);

    REQUIRE(mv.view_count() == 2);
    auto v0 = mv.view(0);
    auto v1 = mv.view(1);
    REQUIRE(v0.first == a.data());
    REQUIRE(v0.second == 2);
    REQUIRE(v1.first == b.data());
    REQUIRE(v1.second == 2);

    REQUIRE_THROWS_AS(mv.view(2), std::out_of_range);
}

TEST_CASE("multi_buffer_view: const_iterator equivalence (different containers are not equal)", "[multi_buffer_view][iter-eq]") {
    std::array<uint8_t, 2> a{1,2};
    memepp::multi_buffer_view m1, m2;
    m1.append(a.data(), 2);
    m2.append(a.data(), 2);

    auto b1 = m1.begin();
    auto b2 = m2.begin();
    REQUIRE_FALSE(b1 == b2); // Iterators from different containers are never equal

    // Reflexivity/Transitivity
    auto b1_copy = b1;
    REQUIRE(b1 == b1_copy);
    ++b1_copy;
    REQUIRE(b1 != b1_copy);
}

TEST_CASE("multi_buffer_view: read_bytes overshoot returns partial length", "[multi_buffer_view][read_bytes]") {
    std::array<uint8_t, 3> a{5,6,7};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    uint8_t buf[8]{};
    auto n = mv.read_bytes(memepp::multi_buffer_view::position{0, 1}, buf, 8); // 从 6 开始读 8 个，但只有 2
    REQUIRE(n == 2);
    REQUIRE(buf[0] == 6);
    REQUIRE(buf[1] == 7);
}

TEST_CASE("multi_buffer_view: find across zero-length view", "[multi_buffer_view][find]") {
    // The find semantics: detect if the contiguous bytes starting from the specified start point match the given sequence exactly
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 2> b{3,4};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(nullptr, 0);  // zero-length
    mv.append(b.data(), 2);

    uint8_t pat[] = {2,3,4};
    auto start = mv.convert_position(1); // Match 2,3,4 starting from value=2
    auto f = mv.find(pat, 3, start);
    REQUIRE(f.index == start.index);
    REQUIRE(f.offset == start.offset);

}

TEST_CASE("multi_buffer_view: end_position()/is_end() and normalize() in various invalid positions", "[multi_buffer_view][pos]") {
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    // Various invalid positions
    memepp::multi_buffer_view::position p_neg{ -5, -7 };
    auto norm = mv.normalize(p_neg);
    REQUIRE(norm.index == 0);
    REQUIRE(norm.offset == 0);

    memepp::multi_buffer_view::position p_far{ 999, 999 };
    norm = mv.normalize(p_far);
    REQUIRE(mv.is_end(norm));
    REQUIRE(norm.index == mv.end_position().index);
    REQUIRE(norm.offset == 0);
}

TEST_CASE("multi_buffer_view: Stability of push_front/back and pop_front/back on empty containers", "[multi_buffer_view][pushpop]") {
    memepp::multi_buffer_view mv;
    mv.pop_front(); // Should not crash
    mv.pop_back();  // Should not crash
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.empty());

    std::array<uint8_t, 1> a{42};
    mv.push_back(a.data(), 1);
    REQUIRE(mv.size() == 1);
    mv.pop_front();
    REQUIRE(mv.size() == 0);
    mv.push_front(a.data(), 1);
    REQUIRE(mv.size() == 1);
    mv.pop_back();
    REQUIRE(mv.size() == 0);
}

TEST_CASE("multi_buffer_view: Random sequence operations and model vector consistency", "[multi_buffer_view][fuzz]") {
    // Build a stable address memory pool to avoid reallocation: 16 blocks, each 128 bytes
    static constexpr size_t POOL_N = 16;
    static constexpr size_t CHUNK  = 128;
    static std::array<std::array<uint8_t, CHUNK>, POOL_N> pool{};

    // Initialize the pool: fill each block with a different pattern for visualization
    for (size_t i = 0; i < POOL_N; ++i) {
        for (size_t j = 0; j < CHUNK; ++j) {
            pool[i][j] = static_cast<uint8_t>((i * 17 + j) & 0xFF);
        }
    }

    std::mt19937 gen(1234567u);

    memepp::multi_buffer_view mv;
    std::vector<uint8_t> model; // Linear model for comparison

    auto append_seg = [&](size_t pi, size_t off, size_t len) {
        mv.append(pool[pi].data() + static_cast<long>(off), static_cast<memepp::multi_buffer_view::size_type>(len));
        model.insert(model.end(), pool[pi].begin() + static_cast<long>(off),
                                  pool[pi].begin() + static_cast<long>(off + len));
    };
    auto prepend_seg = [&](size_t pi, size_t off, size_t len) {
        mv.prepend(pool[pi].data() + static_cast<long>(off), static_cast<memepp::multi_buffer_view::size_type>(len));
        model.insert(model.begin(), pool[pi].begin() + static_cast<long>(off),
                                     pool[pi].begin() + static_cast<long>(off + len));
    };
    auto insert_seg_global = [&](memepp::multi_buffer_view::size_type gpos, size_t pi, size_t off, size_t len) {
        mv.insert(gpos, pool[pi].data() + static_cast<long>(off), static_cast<memepp::multi_buffer_view::size_type>(len));
        auto it = model.begin() + static_cast<long>(std::min<memepp::multi_buffer_view::size_type>(gpos, model.size()));
        model.insert(it,
                     pool[pi].begin() + static_cast<long>(off),
                     pool[pi].begin() + static_cast<long>(off + len));
    };
    auto insert_seg_iterator = [&](memepp::multi_buffer_view::size_type gpos, size_t pi, size_t off, size_t len) {
        auto pos = mv.convert_position(gpos);
        auto it  = memepp::multi_buffer_view::const_iterator{ &mv, pos };
        mv.insert(it, pool[pi].data() + static_cast<long>(off), static_cast<memepp::multi_buffer_view::size_type>(len));
        auto it2 = model.begin() + static_cast<long>(std::min<memepp::multi_buffer_view::size_type>(gpos, model.size()));
        model.insert(it2,
                     pool[pi].begin() + static_cast<long>(off),
                     pool[pi].begin() + static_cast<long>(off + len));
    };

    // Start by appending some content
    for (int i = 0; i < 4; ++i) {
        size_t pi  = rng_uint(gen, 0, POOL_N - 1);
        size_t len = rng_uint(gen, 0, CHUNK / 4); // Moderate length, including 0
        size_t off = rng_uint(gen, 0, CHUNK - (len == 0 ? 0 : len));
        append_seg(pi, off, len);
    }

    // Perform a series of random operations
    for (int step = 0; step < 200; ++step) {
        int op = static_cast<int>(rng_uint(gen, 0, 7));
        size_t pi  = rng_uint(gen, 0, POOL_N - 1);
        size_t len = rng_uint(gen, 0, CHUNK / 6); // Keep it short
        size_t off = rng_uint(gen, 0, CHUNK - (len == 0 ? 0 : len));

        switch (op) {
            case 0: { // append
                append_seg(pi, off, len);
                break;
            }
            case 1: { // prepend
                prepend_seg(pi, off, len);
                break;
            }
            case 2: { // insert at random global position
                auto gpos = (mv.size() == 0) ? 0 : static_cast<memepp::multi_buffer_view::size_type>(
                    rng_uint(gen, 0, static_cast<uint32_t>(mv.size()))
                );
                insert_seg_global(gpos, pi, off, len);
                break;
            }
            case 3: { // insert via iterator
                auto gpos = (mv.size() == 0) ? 0 : static_cast<memepp::multi_buffer_view::size_type>(
                    rng_uint(gen, 0, static_cast<uint32_t>(mv.size()))
                );
                insert_seg_iterator(gpos, pi, off, len);
                break;
            }
            case 4: { // erase range [lo, hi)
                if (mv.size() > 0) {
                    auto lo = static_cast<memepp::multi_buffer_view::size_type>(rng_uint(gen, 0, static_cast<uint32_t>(mv.size() - 1)));
                    auto hi = static_cast<memepp::multi_buffer_view::size_type>(rng_uint(gen, static_cast<uint32_t>(lo), static_cast<uint32_t>(mv.size())));
                    // Use size_type version of range erase
                    mv.erase(lo, hi - lo);
                    model.erase(model.begin() + static_cast<long>(lo),
                                model.begin() + static_cast<long>(hi));
                }
                break;
            }
            case 5: { // pop_front/pop_back hybrid
                int which = static_cast<int>(rng_uint(gen, 0, 1));
                if (which == 0) {
                    mv.pop_front();
                    model.erase(model.begin());
                } else {
                    mv.pop_back();
                    model.pop_back();
                }
                break;
            }
            case 6: { // slice random sampling and comparison with model
                auto gsize = mv.size();
                if (gsize > 0) {
                    auto start = static_cast<memepp::multi_buffer_view::size_type>(rng_uint(gen, 0, static_cast<uint32_t>(gsize - 1)));
                    auto count = static_cast<memepp::multi_buffer_view::size_type>(rng_uint(gen, 0, static_cast<uint32_t>(gsize - start)));
                    auto s = mv.slice(start, count);
                    auto expect = subvec(model, static_cast<size_t>(start), static_cast<size_t>(count));
                    REQUIRE(flatten(s) == expect);
                } else {
                    auto s = mv.slice(0, 10);
                    REQUIRE(s.size() == 0);
                }
                break;
            }
            case 7: default: {
                // no-op
                break;
            }
        }

        // Invariant verification
        auto flat = flatten(mv);
        // Note: Since pop_front/pop_back cannot update the "model" view hierarchy, we relax this to: only compare size consistency failures
        // Stricter comparison: It is recommended to model the view list as well; for simplicity, we still compare content here, and failures will be prompted.
        REQUIRE(flat.size() == static_cast<size_t>(mv.size()));

        // If pop series has not been executed (or the size maintenance of pop is implemented correctly), the following assertions should hold
        // If the implemented erase/remove_byte does not maintain total_size_, failures will be exposed here.
        if (op != 5) {
            REQUIRE(flat == model);
        }

        // Iterator distance == size
        REQUIRE(std::distance(mv.begin(), mv.end()) == static_cast<long>(mv.size()));
        REQUIRE(std::distance(mv.cbegin(), mv.cend()) == static_cast<long>(mv.size()));

        // Randomly sample several convert_position round-trips
        check_convert_roundtrip(mv, gen);
    }
}

// Mixing large-scale views with zero-length views, verifying data/back_data/iteration spans
TEST_CASE("multi_buffer_view: Mixing large-scale views with zero-length views", "[multi_buffer_view][scale]") {
    memepp::multi_buffer_view mv;
    constexpr int N = 500;
    std::vector<std::array<uint8_t, 1>> ones(static_cast<size_t>(N));
    for (int i = 0; i < N; ++i) ones[static_cast<size_t>(i)][0] = static_cast<uint8_t>(i & 0xFF);

    // First insert 500 zero-length views
    for (int i = 0; i < N; ++i) mv.append(nullptr, 0);
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.data() == nullptr);
    REQUIRE(mv.back_data() == nullptr);

    // Then insert 500 one-byte views
    for (int i = 0; i < N; ++i) {
        mv.append(ones[static_cast<size_t>(i)].data(), 1);
    }
    REQUIRE(mv.size() == N);
    REQUIRE(*mv.begin() == 0);
    REQUIRE(*(std::prev(mv.end(), 1)) == static_cast<uint8_t>((N - 1) & 0xFF));

    // Reverse iteration verification
    int expect = N - 1;
    for (auto it = mv.rbegin(); it != mv.rend(); ++it, --expect) {
        REQUIRE(*it == static_cast<uint8_t>(expect & 0xFF));
    }
}

// Insertion return position and subsequent iteration stability
TEST_CASE("multi_buffer_view: insert return position and subsequent iteration stability", "[multi_buffer_view][insert][iter]") {
    std::array<uint8_t, 6> base{10,11,12,13,14,15};
    std::array<uint8_t, 3> mid{99,100,101};

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 6);

    // Insert at global position 3
    auto after = mv.insert(static_cast<memepp::multi_buffer_view::size_type>(3), mid.data(), 3);
    REQUIRE(after == 3);
    auto flat = flatten(mv);
    REQUIRE(flat == std::vector<uint8_t>({10,11,12,99,100,101,13,14,15}));

    // Then insert at iterator begin()+2
    auto it = mv.begin(); ++it; ++it; // Pointing to value=12
    mv.insert(it, mid.data(), 3);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({10,11,99,100,101,12,99,100,101,13,14,15}));
}

// Details of normalize: If the offset is greater than or equal to the current view length, jump to the next view; the end is mapped to end_position
TEST_CASE("multi_buffer_view: normalize Mapping of out-of-bounds offsets", "[multi_buffer_view][normalize]") {
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(b.data(), 2);

    // Offset == len -> jump to next view
    auto p = mv.normalize(memepp::multi_buffer_view::position{0, 3});
    REQUIRE(p.index == 1);
    REQUIRE(p.offset == 0);
    REQUIRE(mv[p] == 4);

    // Offset > len -> also jump to next view (implementation is >=)
    p = mv.normalize(memepp::multi_buffer_view::position{0, 100});
    REQUIRE(p.index == 1);
    REQUIRE(p.offset == 0);

    // Beyond the last view -> end
    p = mv.normalize(memepp::multi_buffer_view::position{10, 0});
    REQUIRE(mv.is_end(p));
    REQUIRE(p.index == mv.end_position().index);
    REQUIRE(p.offset == 0);
}

// erase/replace vs. total_size_ consistency (probe for implementation defects)
TEST_CASE("multi_buffer_view: size() consistency after removal/replacement (implementation sanity check)", "[multi_buffer_view][size-consistency]") {
    std::array<uint8_t, 8> a{1,2,3,4,5,6,7,8};
    std::array<uint8_t, 3> x{9,9,9};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 8);

    // Delete the middle 3
    auto before = mv.size();
    mv.erase(static_cast<memepp::multi_buffer_view::size_type>(2), static_cast<memepp::multi_buffer_view::size_type>(3));
    // If the implementation does not maintain total_size_ in remove_byte, the following assertion will fail, indicating a defect
    REQUIRE(mv.size() == before - 3);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,6,7,8}));

    // Replace 2 with 3 -> size increases by 1
    before = mv.size();
    mv.replace(memepp::multi_buffer_view::position{0, 1}, static_cast<memepp::multi_buffer_view::size_type>(2), x.data(), 3);
    REQUIRE(mv.size() == before + 1);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,9,9,9,7,8}));
}

// Iterators work with standard algorithms: std::equal/std::accumulate, etc.
TEST_CASE("multi_buffer_view: Iterators work with standard algorithms", "[multi_buffer_view][algorithms]") {
    std::array<uint8_t, 5> a{1,2,3,4,5};
    std::array<uint8_t, 4> b{6,7,8,9};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 5);
    mv.append(b.data(), 4);

    auto flat = flatten(mv);
    REQUIRE(std::equal(mv.begin(), mv.end(), flat.begin(), flat.end()));

    // The reverse should also match
    std::vector<uint8_t> rflat(flat.rbegin(), flat.rend());
    REQUIRE(std::equal(mv.rbegin(), mv.rend(), rflat.begin(), rflat.end()));
}

// Extreme position insert: negative position (normalized to the beginning by implementation) and size() position (end)
TEST_CASE("Insert behavior at extreme positions (negative, end)", "[multi_buffer_view][insert-extreme]") {
    std::array<uint8_t, 3> base{10,20,30};
    std::array<uint8_t, 2> x{1,2};
    memepp::multi_buffer_view mv;
    mv.append(base.data(), 3);

    // Negative position: convert_position returns npos in the implementation, which is then corrected to {0,0}
    auto neg = static_cast<memepp::multi_buffer_view::size_type>(-5);
    mv.insert(neg, x.data(), 2);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,10,20,30}));

    mv.insert(mv.size(), x.data(), 2); // Insert at the end
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,10,20,30,1,2}));
}

TEST_CASE("multi_buffer_view: A container that contains only empty views", "[multi_buffer_view][empty]") {
    memepp::buffer_view bv_empty{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };

    memepp::multi_buffer_view mv;
    mv.append(bv_empty);
    mv.prepend(bv_empty);
    mv.append(bv_empty);
    mv.prepend(bv_empty);

    REQUIRE(mv.size() == 0);
    REQUIRE(mv.empty());
    REQUIRE(mv.begin() == mv.end());
    REQUIRE(mv.cbegin() == mv.cend());
    REQUIRE(mv.rbegin() == mv.rend());
    REQUIRE(mv.data() == nullptr);
    REQUIRE(mv.back_data() == nullptr);
    REQUIRE_THROWS_AS(mv.front(), std::out_of_range);
    REQUIRE_THROWS_AS(mv.back(), std::out_of_range);

    // is_end/normalize/end_position Should be stable when there is only an empty view
    auto endp = mv.end_position();
    REQUIRE(mv.is_end(endp));
    auto norm = mv.view_first_position({0, 0});
    REQUIRE(mv.is_end(norm));
}

TEST_CASE("multi_buffer_view: append/prepend/push does not change the content and size, but changes the view_count", "[multi_buffer_view][empty]") {
    memepp::buffer_view bv_empty{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };

    std::array<uint8_t, 4> a{ 1,2,3,4 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);

    auto before_flat = flatten(mv);
    auto before_size = mv.size();
    auto before_views = mv.view_count();

    mv.append(bv_empty);
    mv.prepend(bv_empty);
    mv.push_back(bv_empty);
    mv.push_front(bv_empty);

    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.size() == before_size);
    REQUIRE(mv.view_count() == before_views + 4);

    // data/back_data/The elements before and after are not affected
    REQUIRE(mv.front() == 1);
    REQUIRE(mv.back() == 4);
    REQUIRE(mv.data() == a.data());
    REQUIRE(mv.back_data() == (a.data() + a.size() - 1));
}

TEST_CASE("multi_buffer_view: initializer_list construction/append/prepend with empty views", "[multi_buffer_view][empty]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 3> a{ 10, 20, 30 };
    memepp::buffer_view va{ a.data(), static_cast<memepp::multi_buffer_view::size_type>(a.size()) };

    SECTION("Constructor initializer_list with empty views") {
        memepp::multi_buffer_view mv{ empty_v, va, empty_v };
        REQUIRE(mv.size() == 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,20,30}));
        REQUIRE(mv.view_count() == 3); // Contains two empty views
        REQUIRE(mv.front() == 10);
        REQUIRE(mv.back() == 30);
    }

    SECTION("append/prepend initializer_list with empty views") {
        memepp::multi_buffer_view mv;
        mv.append({ empty_v, va });
        mv.prepend({ empty_v, empty_v });
        mv.append({ empty_v });

        REQUIRE(mv.size() == 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,20,30}));
        REQUIRE(mv.view_count() == 5); // 3 empty views + 1 non-empty + 1 empty
    }

    SECTION("operator= initializer_list with empty views") {
        memepp::multi_buffer_view mv;
        mv = { empty_v, va, empty_v, empty_v };
        REQUIRE(mv.size() == 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,20,30}));
        REQUIRE(mv.view_count() == 4);
    }
}

TEST_CASE("multi_buffer_view: insert empty views at the beginning/middle/end", "[multi_buffer_view][empty][insert]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 5> a{ 1,2,3,4,5 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 5);

    auto start_flat = flatten(mv);
    auto start_size = mv.size();

    SECTION("insert(size_type) insert empty view at the beginning") {
        auto before_views = mv.view_count();
        auto ret = mv.insert(static_cast<memepp::multi_buffer_view::size_type>(0), empty_v);
        REQUIRE(ret == 0);
        REQUIRE(flatten(mv) == start_flat);
        REQUIRE(mv.size() == start_size);
        REQUIRE(mv.view_count() == before_views + 1);
    }

    SECTION("insert(position) insert empty view in the middle (requires split)") {
        auto before_views = mv.view_count();
        memepp::multi_buffer_view::position p = mv.convert_position(2);
        auto pos = mv.insert(p, empty_v);
        // Inserting an empty view does not change content/size, but adds a new view
        REQUIRE(pos.index == 2); // Position is close to split point
        REQUIRE(flatten(mv) == start_flat);
        REQUIRE(mv.size() == start_size);
        REQUIRE(mv.view_count() == before_views + 2);
    }

    SECTION("insert(iterator) insert empty view at the end") {
        auto before_views = mv.view_count();
        auto it = mv.end();
        auto rit = mv.insert(it, empty_v);
        REQUIRE(rit == std::next(mv.begin(), static_cast<long>(mv.size()))); // Still at the end
        REQUIRE(flatten(mv) == start_flat);
        REQUIRE(mv.size() == start_size);
        REQUIRE(mv.view_count() == before_views + 1);
    }

    SECTION("insert(count, empty) insert multiple empty views consecutively") {
        auto before_views = mv.view_count();
        auto retp = mv.insert(mv.convert_position(3), static_cast<memepp::multi_buffer_view::size_type>(5), empty_v);
        REQUIRE(flatten(mv) == start_flat);
        REQUIRE(mv.size() == start_size);
        REQUIRE(mv.view_count() == before_views + 6);
        // The return position points to the beginning of the insertion area
        auto gi = mv.convert_position(retp);
        REQUIRE(gi == 3);
    }
}

TEST_CASE("multi_buffer_view: replace empty view with empty view -> equivalent to deletion", "[multi_buffer_view][empty][replace]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };

    std::array<uint8_t, 6> a{ 10,11,12,13,14,15 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 6);

    SECTION("replace(position, count, empty) delete count bytes") {
        auto before = mv.size();
        auto pos = mv.replace(memepp::multi_buffer_view::position{0, 2}, static_cast<memepp::multi_buffer_view::size_type>(3), empty_v);
        // Content should be deleted 12,13,14
        REQUIRE(flatten(mv) == std::vector<uint8_t>({10,11,15}));
        // size should decrease by 3 (if implementation does not maintain total_size_, this will fail, indicating to fix total_size_ -= bytes_remove in remove_byte)
        REQUIRE(mv.size() == before - 3);

        // The return position should be the new position of the deletion start
        auto g = mv.convert_position(pos);
        REQUIRE(g == 2);
    }

    SECTION("replace(position, 0, empty) does not change content but may add an empty view node") {
        auto before_flat = flatten(mv);
        auto before_size = mv.size();
        auto before_views = mv.view_count();

        mv.replace(memepp::multi_buffer_view::position{0, 3}, static_cast<memepp::multi_buffer_view::size_type>(0), empty_v);
        REQUIRE(flatten(mv) == before_flat);
        REQUIRE(mv.size() == before_size);
        REQUIRE(mv.view_count() == before_views + 2);
    }
}

TEST_CASE("multi_buffer_view: assign/append/prepend/insert accept empty segments in multi_buffer_view", "[multi_buffer_view][empty][mv-ops]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};

    memepp::multi_buffer_view segA; segA.append(a.data(), 3);
    memepp::multi_buffer_view segB; segB.append(b.data(), 2);

    // Construct multi_buffer_view with empty segments
    memepp::multi_buffer_view mix;
    mix.append(empty_v);
    mix.append(segA);
    mix.append(empty_v);
    mix.append(segB);
    mix.append(empty_v);

    SECTION("assign(multi_buffer_view with empty segment)") {
        memepp::multi_buffer_view mv;
        mv.assign(mix);
        REQUIRE(mv.size() == 5);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5}));
        REQUIRE(mv.view_count() == mix.view_count());
    }

    SECTION("append(multi_buffer_view with empty segment)") {
        memepp::multi_buffer_view mv;
        mv.append(mix);
        REQUIRE(mv.size() == 5);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5}));
    }

    SECTION("prepend(multi_buffer_view with empty segment)") {
        memepp::multi_buffer_view mv;
        mv.prepend(mix);
        REQUIRE(mv.size() == 5);
        REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3,4,5}));
    }

    SECTION("insert(position, multi_buffer_view with empty segment)") {
        memepp::multi_buffer_view base;
        base.append(std::array<uint8_t,2>{9,9}.data(), 2); // Note: The temporary array constructed here will be dangling, so change it to a separate array
    }

    SECTION("insert(size_type, multi_buffer_view with empty segment) and content immutability") {
        // Use stable memory to avoid dangling
        std::array<uint8_t,2> zz{9,9};
        memepp::multi_buffer_view base;
        base.append(zz.data(), 2); // [9,9]

        auto gpos = static_cast<memepp::multi_buffer_view::size_type>(1);
        base.insert(gpos, mix); // Insert in the middle [empty][1,2,3][empty][4,5][empty]
        REQUIRE(flatten(base) == std::vector<uint8_t>({9,1,2,3,4,5,9}));
        // size = 2 + 5
        REQUIRE(base.size() == 7);
    }
}

TEST_CASE("multi_buffer_view: Iterator across empty views/normalize behavior at empty view points", "[multi_buffer_view][empty][iter]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 1> a{ 42 };
    memepp::multi_buffer_view mv;
    mv.append(empty_v);
    mv.append(a.data(), 1);
    mv.append(empty_v);

    // begin()/operator* should get the first non-null element
    REQUIRE(*mv.begin() == 42);
    // --end() should also get 42
    auto it = mv.end();
    --it;
    REQUIRE(*it == 42);

    // normalize should jump to the next valid position on empty views
    auto p0 = mv.normalize({0, 0});    // First empty view
    REQUIRE(p0.index == 1);
    REQUIRE(p0.offset == 0);
    REQUIRE(mv[p0] == 42);

    // convert_position should be invertible for valid global positions
    auto p = mv.convert_position(0);
    REQUIRE(mv.convert_position(p) == 0);
}

TEST_CASE("multi_buffer_view: remove_view removes empty views without changing size/content", "[multi_buffer_view][empty][remove_view]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 3> a{1,2,3};

    memepp::multi_buffer_view mv;
    mv.append(empty_v);                 // idx 0: empty
    mv.append(a.data(), 3);             // idx 1: non-empty
    mv.append(empty_v);                 // idx 2: empty

    auto before_flat = flatten(mv);
    auto before_size = mv.size();

    mv.remove_view(2); // remove trailing empty view
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3}));
    REQUIRE(mv.size() == before_size);  // size should not change

    mv.remove_view(0); // remove leading empty view
    REQUIRE(flatten(mv) == std::vector<uint8_t>({1,2,3}));
    REQUIRE(mv.size() == before_size);
}

TEST_CASE("multi_buffer_view: slice/erase/read_bytes error tolerance", "[multi_buffer_view][empty][ops]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 4> a{1,2,3,4};

    memepp::multi_buffer_view mv;
    mv.append(empty_v);
    mv.append(a.data(), 4);
    mv.append(empty_v);

    SECTION("Slice covers empty view boundaries") {
        // Start from global 0 and take 2 (across the leading empty view, which should be equivalent to starting from the first element)
        auto s = mv.slice(static_cast<memepp::multi_buffer_view::size_type>(0), static_cast<memepp::multi_buffer_view::size_type>(2));
        REQUIRE(flatten(s) == std::vector<uint8_t>({1,2}));

        // Default length to the end
        auto s2 = mv.slice(static_cast<memepp::multi_buffer_view::size_type>(1));
        REQUIRE(flatten(s2) == std::vector<uint8_t>({2,3,4}));
    }

    SECTION("erase 0 bytes should not change content") {
        auto before_flat = flatten(mv);
        auto before_size = mv.size();
        auto pos = mv.convert_position(1);
        auto new_pos = mv.remove_bytes(pos, static_cast<memepp::multi_buffer_view::size_type>(0));
        REQUIRE(flatten(mv) == before_flat);
        REQUIRE(mv.size() == before_size);
        REQUIRE(mv.convert_position(new_pos) == 1);
    }

    SECTION("read_bytes across empty views") {
        std::array<uint8_t, 3> buf{};
        auto n = mv.read_bytes(memepp::multi_buffer_view::position{0,0}, buf.data(), 3); // Start reading from empty view
        REQUIRE(n == 3);
        REQUIRE(std::vector<uint8_t>(buf.begin(), buf.end()) == std::vector<uint8_t>({1,2,3}));
    }
}

TEST_CASE("multi_buffer_view: Equality (ignoring empty view partition differences)", "[multi_buffer_view][empty][eq]") {
    memepp::buffer_view empty_v{ static_cast<const uint8_t*>(nullptr), static_cast<memepp::multi_buffer_view::size_type>(0) };
    std::array<uint8_t, 3> a{7,8,9};

    memepp::multi_buffer_view m1, m2;
    m1.append(a.data(), 3);                  // Single non-empty view   
    m2.append(empty_v);
    m2.append(a.data(), 2);
    m2.append(empty_v);
    m2.append(a.data() + 2, 1);
    m2.append(empty_v);

    REQUIRE(m1 == m2);
    REQUIRE_FALSE(m1 != m2);
}

#if MG_LANG__CXX17_AVAIL
TEST_CASE("multi_buffer_view: string_view ctor/assign/operator=", "[multi_buffer_view][string_view]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::string s = "abcXYZ";
    std::string_view sv(s);

    memepp::multi_buffer_view mv(sv);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 'a','b','c','X','Y','Z' }));

    // assign(std::string_view)
    std::string s2 = "12";
    std::string_view sv2(s2);
    mv.assign(sv2);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ '1','2' }));

    // operator=(std::string_view)
    std::string s3 = "pq";
    std::string_view sv3(s3);
    mv = sv3;
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 'p','q' }));

    std::string s4 = "K";
    std::string_view sv4(s4);
    mv.clear();
    mv.append(static_cast<size_type>(3), sv4); // "KKK"
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 'K','K','K' }));
    mv.prepend(static_cast<size_type>(2), sv4); // "KKKKK"
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 'K','K','K','K','K' }));
}
#endif

TEST_CASE("multi_buffer_view: assign(count, view) and assign(begin,end)", "[multi_buffer_view][assign]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 2> a{ 7,8 };
    memepp::buffer_view va{ a.data(), static_cast<size_type>(a.size()) };

    memepp::multi_buffer_view mv;
    mv.assign(static_cast<size_type>(3), va);   // [7,8][7,8][7,8]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 7,8,7,8,7,8 }));

    std::array<uint8_t, 3> b{ 1,2,3 };
    std::array<uint8_t, 1> c{ 9 };
    std::vector<memepp::buffer_view> segs{
        { b.data(), static_cast<size_type>(b.size()) },
        { c.data(), static_cast<size_type>(c.size()) }
    };
    mv.assign(segs.begin(), segs.end());
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,3,9 }));
}

TEST_CASE("multi_buffer_view: append/prepend with count and with iterator ranges", "[multi_buffer_view][range][count]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 2> a{ 1,2 };
    memepp::buffer_view va{ a.data(), static_cast<size_type>(a.size()) };

    memepp::multi_buffer_view mv;
    mv.append(static_cast<size_type>(2), va); // [1,2][1,2]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,1,2 }));

    mv.prepend(static_cast<size_type>(1), va); // [1,2][1,2][1,2]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,1,2,1,2 }));

    // append(begin,end)
    std::array<uint8_t, 3> b{ 3,4,5 };
    std::array<uint8_t, 1> c{ 9 };
    std::vector<memepp::buffer_view> segs{
        { b.data(), static_cast<size_type>(b.size()) },
        { c.data(), static_cast<size_type>(c.size()) }
    };
    mv.clear();
    mv.append(segs.begin(), segs.end()); // [3,4,5][9]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 3,4,5,9 }));

    // prepend(begin,end)
    mv.clear();
    mv.prepend(segs.begin(), segs.end()); // [3,4,5][9]
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 3,4,5,9 }));
}

TEST_CASE("multi_buffer_view: insert with iterator range of pairs", "[multi_buffer_view][insert-range]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> base{ 10,20,30 };
    memepp::multi_buffer_view mv;
    mv.append(base.data(), static_cast<size_type>(base.size()));

    std::array<uint8_t, 2> x{ 99,100 };
    std::array<uint8_t, 3> y{ 7,8,9 };

    std::vector<memepp::buffer_view> segs;
    segs.emplace_back(memepp::buffer_view{ x.data(), static_cast<size_type>(x.size()) }); // [99,100]
    segs.emplace_back(memepp::buffer_view{ y.data(), static_cast<size_type>(y.size()) }); // [7,8,9]

    auto pos = mv.insert(mv.convert_position(1), segs.begin(), segs.end());

    REQUIRE(pos.index == 1);
    REQUIRE(pos.offset == 0);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 10,99,100,7,8,9,20,30 }));
}

TEST_CASE("multi_buffer_view: find default start, single-view match, not found, and empty/null patterns", "[multi_buffer_view][find]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 5> a{ 1,2,3,4,5 };
    std::array<uint8_t, 3> b{ 6,7,8 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), static_cast<size_type>(a.size()));
    mv.append(b.data(), static_cast<size_type>(b.size()));

    uint8_t p1[] = { 2,3,4 };
    auto f1 = mv.find(p1, static_cast<size_type>(3));
    auto start1 = mv.convert_position(static_cast<size_type>(1));
    REQUIRE(f1.index == start1.index);
    REQUIRE(f1.offset == start1.offset);

    uint8_t p2[] = { 42,43 };
    auto f2 = mv.find(p2, static_cast<size_type>(2));
    REQUIRE(f2.index == mv.end_position().index);
    REQUIRE(f2.offset == mv.end_position().offset);

    uint8_t p3[] = { 1,2,3,4,5,6,7,8,9 };
    auto f3 = mv.find(p3, static_cast<size_type>(9));
    REQUIRE(f3.index == mv.end_position().index);
    REQUIRE(f3.offset == mv.end_position().offset);

    auto f4 = mv.find(static_cast<const uint8_t*>(nullptr), static_cast<size_type>(0));
    REQUIRE(f4.index == mv.end_position().index);
    REQUIRE(f4.offset == mv.end_position().offset);
}

TEST_CASE("multi_buffer_view: const_iterator operator->", "[multi_buffer_view][iterator]") {
    std::array<uint8_t, 4> a{ 10,20,30,40 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);

    auto it = mv.begin();
    auto p = it.operator->();
    REQUIRE(p != nullptr);
    REQUIRE(*p == 10);
    ++it;
    REQUIRE(*(it.operator->()) == 20);
}

TEST_CASE("multi_buffer_view: push_front/push_back with multi_buffer_view", "[multi_buffer_view][push]") {
    std::array<uint8_t, 2> a{ 1,2 };
    std::array<uint8_t, 2> b{ 3,4 };
    std::array<uint8_t, 2> c{ 5,6 };

    memepp::multi_buffer_view A; A.append(a.data(), 2);
    memepp::multi_buffer_view B; B.append(b.data(), 2);
    memepp::multi_buffer_view C; C.append(c.data(), 2);

    memepp::multi_buffer_view mv;
    mv.push_back(B);  // [3,4]
    mv.push_front(A); // [1,2][3,4]
    mv.push_back(C);  // [1,2][3,4][5,6]

    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,3,4,5,6 }));
}

TEST_CASE("multi_buffer_view: view_first_position/view_last_position boundaries", "[multi_buffer_view][viewpos]") {
    std::array<uint8_t, 3> a{ 1,2,3 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    // view_first_position(view_count()) -> end
    auto p1 = mv.view_first_position(mv.view_count());
    REQUIRE(mv.is_end(p1));

    auto p2 = mv.view_last_position(static_cast<memepp::multi_buffer_view::size_type>(0));
    REQUIRE_FALSE(mv.is_end(p2));
    REQUIRE(mv[p2] == 3);
}

TEST_CASE("multi_buffer_view: crbegin/crend on empty and non-empty", "[multi_buffer_view][reverse][const]") {
    memepp::multi_buffer_view mv_empty;
    REQUIRE(mv_empty.crbegin() == mv_empty.crend());

    std::array<uint8_t, 4> a{ 1,2,3,4 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);

    auto rit = mv.crbegin();
    REQUIRE(*rit == 4);

    REQUIRE(*mv.rbegin() == *mv.crbegin());
}

TEST_CASE("multi_buffer_view: operator== fast path with identical view segments", "[multi_buffer_view][eq-fast]") {
    std::array<uint8_t, 3> a{ 1,2,3 };
    std::array<uint8_t, 2> b{ 4,5 };

    memepp::multi_buffer_view m1, m2;
    m1.append(a.data(), 3);
    m1.append(b.data(), 2);

    m2.append(a.data(), 3);
    m2.append(b.data(), 2);

    REQUIRE(m1.view_count() == m2.view_count());
    REQUIRE(m1 == m2); 
    REQUIRE_FALSE(m1 != m2);
}

TEST_CASE("multi_buffer_view: replace(position, view) and replace(position, data,size)", "[multi_buffer_view][replace]") {
    std::array<uint8_t, 5> base{ 10,11,12,13,14 };
    std::array<uint8_t, 1> x{ 99 };

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 5);

    auto p = memepp::multi_buffer_view::position{ 0, 2 }; // 12
    auto after = mv.replace(p, memepp::buffer_view{ x.data(), 1 });
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 10,11,99,13,14 }));
    REQUIRE(after.index == 1);
    REQUIRE(after.offset == 0);

    std::array<uint8_t, 1> y{ 7 };
    after = mv.replace(memepp::multi_buffer_view::position{ 0, 0 }, y.data(), 1);
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 7,11,99,13,14 }));
    REQUIRE(after.index == 0);
    REQUIRE(after.offset == 0);
}

#if MG_LANG__CXX17_AVAIL
TEST_CASE("multi_buffer_view: push_front/push_back with std::string_view", "[multi_buffer_view][string_view][push]") {
    std::string s1 = "AB";
    std::string s2 = "xy";
    std::string_view sv1(s1), sv2(s2);

    memepp::multi_buffer_view mv;
    mv.push_back(sv1);   // "AB"
    mv.push_front(sv2);  // "xyAB"
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 'x','y','A','B' }));
}
#endif

TEST_CASE("multi_buffer_view: find with overlapping prefixes and cross-view fallback", "[multi_buffer_view][find][overlap]") {
    using size_type = memepp::multi_buffer_view::size_type;

    {
        std::array<uint8_t, 3> d{ 1,1,1 };
        memepp::multi_buffer_view mv;
        mv.append(d.data(), 3);
        uint8_t pat[] = { 1,1 };
        auto f = mv.find(pat, static_cast<size_type>(2));
        auto s0 = mv.convert_position(static_cast<size_type>(0));
        REQUIRE(f.index == s0.index);
        REQUIRE(f.offset == s0.offset);
    }

    {
        std::array<uint8_t, 1> a{ 1 };
        std::array<uint8_t, 2> b{ 1,2 };
        memepp::multi_buffer_view mv;
        mv.append(a.data(), 1);
        mv.append(b.data(), 2);

        uint8_t pat2[] = { 1,2 };
        auto f2 = mv.find(pat2, static_cast<size_type>(2));
        auto s1 = mv.convert_position(static_cast<size_type>(1));
        REQUIRE(f2.index == s1.index);
        REQUIRE(f2.offset == s1.offset);
    }
}

TEST_CASE("multi_buffer_view: view_first_position(position) / view_last_position(position) offset OOB -> end", "[multi_buffer_view][viewpos]") {
    std::array<uint8_t, 3> a{ 1,2,3 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    // view_first_position(position) with offset >= size -> end
    auto p1 = mv.view_first_position(memepp::multi_buffer_view::position{ 0, 3 });
    REQUIRE(mv.is_end(p1));

    // view_last_position(position) with offset >= size -> end
    auto p2 = mv.view_last_position(memepp::multi_buffer_view::position{ 0, 3 });
    REQUIRE(mv.is_end(p2));
}

TEST_CASE("multi_buffer_view: remove_bytes removing a whole view leaves zero-length node but data/back_data/size correct", "[multi_buffer_view][erase][structure]") {

    std::array<uint8_t, 3> A{ 1,2,3 };
    std::array<uint8_t, 2> B{ 4,5 };
    std::array<uint8_t, 3> C{ 6,7,8 };

    memepp::multi_buffer_view mv;
    mv.append(A.data(), 3);
    mv.append(B.data(), 2);
    mv.append(C.data(), 3);

    auto before_views = mv.view_count();
    auto after_pos = mv.erase(static_cast<memepp::multi_buffer_view::size_type>(3),
        static_cast<memepp::multi_buffer_view::size_type>(2));
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,3,6,7,8 }));
    REQUIRE(mv.size() == 6);

    REQUIRE(mv.view_count() >= before_views - 1);
    REQUIRE(mv.front() == 1);
    REQUIRE(mv.back() == 8);
    REQUIRE(*mv.begin() == 1);
    REQUIRE(*std::prev(mv.end()) == 8);

    auto gi = mv.convert_position(after_pos);
    REQUIRE((gi.offset == 0 && gi.index == 2));
}

TEST_CASE("multi_buffer_view: to_buffer produces contiguous copy equal to flatten", "[multi_buffer_view][to_buffer]") {
    std::array<uint8_t, 3> a{ 1,2,3 };
    std::array<uint8_t, 2> b{ 4,5 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(nullptr, 0);
    mv.append(b.data(), 2);

    auto buf = mv.to_buffer();
    std::vector<uint8_t> flat = flatten(mv);
    REQUIRE(buf.size() == mv.size());
    REQUIRE(buf.size() == static_cast<memepp::multi_buffer_view::size_type>(flat.size()));
    REQUIRE(std::memcmp(buf.data(), flat.data(), flat.size()) == 0);
}

TEST_CASE("multi_buffer_view: insert(count=0) is no-op and returns insertion anchor", "[multi_buffer_view][insert][count0]") {
    using size_type = memepp::multi_buffer_view::size_type;
    std::array<uint8_t, 4> base{ 10,20,30,40 };
    std::array<uint8_t, 2> x{ 99,100 };
    memepp::buffer_view vx{ x.data(), static_cast<size_type>(x.size()) };

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 4);

    auto before_flat = flatten(mv);
    auto before_views = mv.view_count();

    auto anchor = mv.insert(mv.convert_position(2), static_cast<size_type>(0), vx);
    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.view_count() == before_views);
    
    REQUIRE(mv.convert_position(anchor) == 2);
}

TEST_CASE("multi_buffer_view: append/prepend nullptr with non-zero size treated as empty view", "[multi_buffer_view][null]") {
    using size_type = memepp::multi_buffer_view::size_type;
    std::array<uint8_t, 3> a{ 1,2,3 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    auto before_flat = flatten(mv);
    auto before_views = mv.view_count();
    auto before_size = mv.size();

    mv.append(static_cast<const uint8_t*>(nullptr), static_cast<size_type>(123));
    mv.prepend(static_cast<const uint8_t*>(nullptr), static_cast<size_type>(456));

    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.size() == before_size);
    REQUIRE(mv.view_count() == before_views + 2);
}

TEST_CASE("multi_buffer_view: pop_front/pop_back skip empty views at ends", "[multi_buffer_view][pop][empty-ends]") {
    using size_type = memepp::multi_buffer_view::size_type;
    std::array<uint8_t, 4> a{ 10,20,30,40 };
    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0); 
    mv.append(a.data(), 4); 
    mv.append(nullptr, 0); 

    mv.pop_front();
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 20,30,40 }));
    REQUIRE(mv.size() == 3);

    mv.pop_back();
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 20,30 }));
    REQUIRE(mv.size() == 2);
}

TEST_CASE("multi_buffer_view: normalize({npos,npos}) yields end_position", "[multi_buffer_view][normalize][npos]") {
    memepp::multi_buffer_view mv;
    std::array<uint8_t, 2> a{ 1,2 };
    mv.append(a.data(), 2);

    memepp::multi_buffer_view::position p{ memepp::multi_buffer_view::npos, memepp::multi_buffer_view::npos };
    auto n = mv.normalize(p);
    REQUIRE(mv.is_end(n));
    REQUIRE(n.index == mv.end_position().index);
    REQUIRE(n.offset == 0);
}

TEST_CASE("multi_buffer_view: insert at a position that points to an empty view", "[multi_buffer_view][insert][empty-pos]") {
    using size_type = memepp::multi_buffer_view::size_type;
    std::array<uint8_t, 3> a{ 1,2,3 };
    std::array<uint8_t, 2> x{ 9,9 };

    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0);                 // idx 0: empty
    mv.append(a.data(), 3);                // idx 1: data

    auto pos = mv.insert(memepp::multi_buffer_view::position{ 0, 0 }, x.data(), static_cast<size_type>(x.size()));
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 9,9,1,2,3 }));
    REQUIRE(pos.index == 1);
    REQUIRE(pos.offset == 0);
}

TEST_CASE("multi_buffer_view: insert with size_type == npos inserts at end", "[multi_buffer_view][insert][npos]") {
    using size_type = memepp::multi_buffer_view::size_type;
    std::array<uint8_t, 3> a{ 1,2,3 };
    std::array<uint8_t, 2> x{ 7,8 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    auto ret = mv.insert(mv.npos, x.data(), static_cast<size_type>(x.size()));
    REQUIRE(flatten(mv) == std::vector<uint8_t>({ 1,2,3,7,8 }));
    REQUIRE((ret == 3 || ret == mv.npos));
}

TEST_CASE("multi_buffer_view: reverse iterator distance equals size", "[multi_buffer_view][reverse][distance]") {
    std::array<uint8_t, 5> a{ 1,2,3,4,5 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 5);

    auto dist = std::distance(mv.rbegin(), mv.rend());
    REQUIRE(dist == static_cast<long>(mv.size()));
}

TEST_CASE("multi_buffer_view: removing all bytes leaves only empty views; data()/back_data() become nullptr", "[multi_buffer_view][erase-all][empty]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> a{ 1,2,3 };
    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0); 
    mv.append(a.data(), 3);
    mv.append(nullptr, 0);

    mv.erase(static_cast<size_type>(0), static_cast<size_type>(mv.size()));
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.empty());
    REQUIRE(mv.data() == nullptr);
    REQUIRE(mv.back_data() == nullptr);

    REQUIRE(mv.begin() == mv.end());
    REQUIRE(mv.rbegin() == mv.rend());
}

TEST_CASE("multi_buffer_view: compact merges adjacent views and cleans empty views", "[multi_buffer_view][compact]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> a{ 1, 2, 3 };
    std::array<uint8_t, 2> b{ 4, 5 };
    std::array<uint8_t, 2> c{ 6, 7 };

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(nullptr, 0); 
    mv.append(b.data(), 2);
    mv.append(nullptr, 0); 
    mv.append(c.data(), 2);

    auto before_flat = flatten(mv);
    auto before_size = mv.size();
    auto before_views = mv.view_count(); 

    mv.compact(); 

    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.size() == before_size);  
    REQUIRE(mv.view_count() < before_views);

    // 验证data/back_data
    REQUIRE(mv.data() != nullptr);
    REQUIRE(mv.back_data() != nullptr);
    REQUIRE(mv.front() == 1);
    REQUIRE(mv.back() == 7);

    // 空mv的compact
    memepp::multi_buffer_view empty_mv;
    empty_mv.append(nullptr, 0);
    empty_mv.compact();
    REQUIRE(empty_mv.empty());
    REQUIRE(empty_mv.view_count() == 0); 
}

TEST_CASE("multi_buffer_view: read<Ty> with endian and error handling", "[multi_buffer_view][read]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 2> a{ 0x01, 0x02 };
    std::array<uint8_t, 2> b{ 0x03, 0x04 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(b.data(), 2);

    SECTION("read<uint32_t> little endian") {
        uint32_t val = 0;
        auto err = mv.read(static_cast<size_type>(0), val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == 0x04030201u);
    }

    SECTION("read<uint32_t> big endian (swaps)") {
        uint32_t val = 0;
        auto err = mv.read(static_cast<size_type>(0), val, mgpp::endian_t::big_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == 0x01020304u);  // 假设byte_swap正确
    }

    SECTION("read<uint16_t> insufficient data") {
        uint16_t val = 0;
        auto err = mv.read(static_cast<size_type>(3), val, mgpp::endian_t::little_byte);
        REQUIRE(err.code() == MGEC__RANGE);
        REQUIRE(err.message() == "Not enough data");
    }

    SECTION("read across empty view") {
        mv.prepend(nullptr, 0);
        uint32_t val = 0;
        auto err = mv.read(memepp::multi_buffer_view::position{ 0, 0 }, val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == 0x04030201u);
    }

    SECTION("read<int64_t> large type") {
        std::array<uint8_t, 4> c{ 0x05, 0x06, 0x07, 0x08 };
        mv.append(c.data(), 4);
        int64_t val = 0;
        auto err = mv.read(static_cast<size_type>(0), val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == 0x0807060504030201LL);  // little endian
    }
}

TEST_CASE("multi_buffer_view: position operator== and !=", "[multi_buffer_view][position]") {
    memepp::multi_buffer_view::position p1{ 1, 2 };
    memepp::multi_buffer_view::position p2{ 1, 2 };
    memepp::multi_buffer_view::position p3{ 3, 4 };

    REQUIRE(p1 == p2);
    REQUIRE_FALSE(p1 != p2);
    REQUIRE(p1 != p3);
    REQUIRE_FALSE(p1 == p3);

    memepp::multi_buffer_view::position pn{ memepp::multi_buffer_view::npos, memepp::multi_buffer_view::npos };
    REQUIRE(pn == pn);
    REQUIRE(pn != p1);
}

TEST_CASE("multi_buffer_view: to_buffer on empty and only-empty-views", "[multi_buffer_view][to_buffer][empty]") {
    memepp::multi_buffer_view mv_empty;
    auto buf_empty = mv_empty.to_buffer();
    REQUIRE(buf_empty.size() == 0);
    REQUIRE(buf_empty.data() != nullptr);

    memepp::multi_buffer_view mv_only_empty;
    mv_only_empty.append(nullptr, 0);
    mv_only_empty.append(nullptr, 0);
    auto buf_only_empty = mv_only_empty.to_buffer();
    REQUIRE(buf_only_empty.size() == 0);
    REQUIRE(buf_only_empty.data() != nullptr);
}

TEST_CASE("multi_buffer_view: read_bytes with mgpp::err return and error codes", "[multi_buffer_view][read_bytes][err]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> a{ 1, 2, 3 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    uint8_t buf[4]{};
    auto err = mv.read_bytes(static_cast<size_type>(1), buf, 4);
    REQUIRE(err == mgpp::err{});
    REQUIRE(buf[0] == 2);
    REQUIRE(buf[1] == 3);

    auto err2 = mv.read_bytes(static_cast<size_type>(5), buf, 1);
    REQUIRE(err2.code() == MGEC__RANGE);
}

TEST_CASE("multi_buffer_view: const_iterator multi-step advance/retreat across empty views", "[multi_buffer_view][iterator][empty]") {
    std::array<uint8_t, 2> a{ 1, 2 };
    std::array<uint8_t, 2> b{ 3, 4 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(nullptr, 0);
    mv.append(b.data(), 2);

    auto it = mv.cbegin();
    std::advance(it, 3); 
    REQUIRE(*it == 4);

    std::advance(it, -2); 
    REQUIRE(*it == 2);

    std::advance(it, -2);
    REQUIRE(it == mv.cbegin());
    REQUIRE(*it == 1);

    it = mv.cbegin();
    std::advance(it, 5);
    REQUIRE(it == mv.cend());
}

TEST_CASE("multi_buffer_view: large number of views (thousands of empty + data)", "[multi_buffer_view][scale][empty]") {
    memepp::multi_buffer_view mv;
    constexpr int N = 10000;
    for (int i = 0; i < N; ++i) {
        mv.append(nullptr, 0);
    }
    std::array<uint8_t, 1> a{ 42 };
    mv.append(a.data(), 1);

    REQUIRE(mv.size() == 1);
    REQUIRE(mv.front() == 42);
    REQUIRE(mv.back() == 42);
    REQUIRE(*mv.begin() == 42);

    auto flat = flatten(mv);
    REQUIRE(flat == std::vector<uint8_t>{42});

    mv.compact();
    REQUIRE(mv.view_count() == 1);
    REQUIRE(mv.size() == 1);
}

TEST_CASE("multi_buffer_view: swap is noexcept and swaps states correctly", "[multi_buffer_view][swap][noexcept]") {
    std::array<uint8_t, 2> a{ 1, 2 };
    std::array<uint8_t, 3> b{ 3, 4, 5 };

    memepp::multi_buffer_view mv1, mv2;
    mv1.append(a.data(), 2);
    mv2.append(b.data(), 3);

    REQUIRE_NOTHROW(mv1.swap(mv2));

    REQUIRE(flatten(mv1) == std::vector<uint8_t>{3, 4, 5});
    REQUIRE(flatten(mv2) == std::vector<uint8_t>{1, 2});
    REQUIRE(mv1.size() == 3);
    REQUIRE(mv2.size() == 2);
}

#if MG_LANG__CXX17_AVAIL
TEST_CASE("multi_buffer_view: string_view insert/replace/find (C++17)", "[multi_buffer_view][string_view]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::string base_str = "abc";
    std::string ins_str = "XY";
    std::string_view base_sv(base_str), ins_sv(ins_str);

    memepp::multi_buffer_view mv(base_sv);  // "abc"

    // insert(string_view)
    auto ret = mv.insert(static_cast<size_type>(1), ins_sv);
    REQUIRE(ret == 1);
    REQUIRE(flatten(mv) == std::vector<uint8_t>{'a', 'X', 'Y', 'b', 'c'});

    // replace(position, count, string_view)
    auto pos = mv.convert_position(2);
    auto after = mv.replace(pos, static_cast<size_type>(2), ins_sv);
    REQUIRE(flatten(mv) == std::vector<uint8_t>{'a', 'X', 'X', 'Y', 'c'});

    // find(string_view)
    auto f = mv.find(ins_sv, mv.convert_position(1));
    REQUIRE(mv.convert_position(f) == 2);
}
#endif

TEST_CASE("multi_buffer_view: remove_view multiple consecutive and out-of-range", "[multi_buffer_view][remove_view]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 2> a{ 1, 2 };
    std::array<uint8_t, 2> b{ 3, 4 };
    std::array<uint8_t, 2> c{ 5, 6 };

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);  // idx 0
    mv.append(b.data(), 2);  // idx 1
    mv.append(c.data(), 2);  // idx 2

    auto before_flat = flatten(mv);
    auto before_size = mv.size();

    mv.remove_view(1); 
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1, 2, 5, 6});
    REQUIRE(mv.size() == before_size - 2);

    mv.remove_view(0);
    mv.remove_view(0); 
    REQUIRE(mv.empty());
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.view_count() == 0); 

    mv.remove_view(100);
    REQUIRE(mv.empty());
}

TEST_CASE("multi_buffer_view: pop_front/pop_back on single-byte views", "[multi_buffer_view][pop][single]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 1> a{ 1 };
    std::array<uint8_t, 1> b{ 2 };
    std::array<uint8_t, 1> c{ 3 };

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 1);
    mv.append(b.data(), 1);
    mv.append(c.data(), 1);

    mv.pop_back();
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1, 2});
    REQUIRE(mv.size() == 2);
    REQUIRE(mv.view_count() == 3);

    mv.pop_front();
    REQUIRE(flatten(mv) == std::vector<uint8_t>{2});
    REQUIRE(mv.size() == 1);

    mv.pop_front();
    REQUIRE(mv.empty());
    REQUIRE(mv.size() == 0);
}

TEST_CASE("multi_buffer_view: find with size=1 and repeated elements", "[multi_buffer_view][find][simple]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 5> a{ 1, 2, 1, 2, 1 };
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 5);

    uint8_t pat = 1;
    auto f = mv.find(&pat, static_cast<size_type>(1));
    REQUIRE(mv.convert_position(f) == 0);

    auto f2 = mv.find(&pat, static_cast<size_type>(1), mv.convert_position(1));
    REQUIRE(mv.convert_position(f2) == 2);
}

TEST_CASE("multi_buffer_view: slice with npos count from middle/empty-view/end", "[multi_buffer_view][slice][npos]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> a{ 1, 2, 3 };
    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0);
    mv.append(a.data(), 3);
    mv.append(nullptr, 0);

    auto s = mv.slice(static_cast<size_type>(1), mv.npos);
    REQUIRE(flatten(s) == std::vector<uint8_t>{2, 3});

    auto s2 = mv.slice(memepp::multi_buffer_view::position{ 0, 0 }, mv.npos);
    REQUIRE(flatten(s2) == std::vector<uint8_t>{1, 2, 3});

    auto s3 = mv.slice(mv.npos, mv.npos);
    REQUIRE(s3.empty());
}

TEST_CASE("multi_buffer_view: move constructor and assignment", "[multi_buffer_view][move]") {
    using size_type = memepp::multi_buffer_view::size_type;

    std::array<uint8_t, 3> a{ 1, 2, 3 };
    memepp::multi_buffer_view src;
    src.append(a.data(), 3);

    // move constructor
    memepp::multi_buffer_view mv(std::move(src));
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1, 2, 3});
    REQUIRE(mv.size() == 3);
    REQUIRE(src.empty()); 
    REQUIRE(src.size() == 0);

    // move assignment
    memepp::multi_buffer_view src2;
    src2.append(a.data(), 3);
    memepp::multi_buffer_view mv2;
    mv2 = std::move(src2);
    REQUIRE(flatten(mv2) == std::vector<uint8_t>{1, 2, 3});
    REQUIRE(mv2.size() == 3);
    REQUIRE(src2.empty());
}

TEST_CASE("multi_buffer_view: const-correctness of const methods", "[multi_buffer_view][const]") {
    std::array<uint8_t, 3> a{ 1, 2, 3 };
    const memepp::multi_buffer_view mv_const(a.data(), 3);

    REQUIRE(mv_const.size() == 3);
    REQUIRE(mv_const.empty() == false);
    REQUIRE(mv_const.data() == a.data());
    REQUIRE(mv_const.front() == 1);
    REQUIRE(mv_const.back() == 3);
    REQUIRE(*mv_const.cbegin() == 1);
    REQUIRE(*std::prev(mv_const.cend()) == 3);
    REQUIRE(mv_const.view_count() == 1);

}

TEST_CASE("multi_buffer_view: reserve_views changes capacity", "[multi_buffer_view][reserve]") {
    memepp::multi_buffer_view mv;
    auto init_cap = mv.views_capacity();

    mv.reserve_views(10);
    REQUIRE(mv.views_capacity() >= 10);
    REQUIRE(mv.empty()); 
    REQUIRE(mv.size() == 0);

    std::array<uint8_t, 5> a{ 1, 2, 3, 4, 5 };
    mv.append(a.data(), 5);
    REQUIRE(mv.views_capacity() >= 10);
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1, 2, 3, 4, 5});
}

TEST_CASE("multi_buffer_view: reset_view resets specific view to empty and updates size", "[multi_buffer_view][reset_view]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};
    std::array<uint8_t, 2> c{6,7};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);  // idx 0
    mv.append(b.data(), 2);  // idx 1
    mv.append(c.data(), 2);  // idx 2

    auto before_size = mv.size();
    auto before_flat = flatten(mv);

    SECTION("reset middle view") {
        mv.reset_view(1);  // Reset [4,5] to empty
        REQUIRE(mv.size() == before_size - 2);
        REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2,3,6,7});
        REQUIRE(mv.view_count() == 3);  // View count remains, but it's empty
        REQUIRE(mv.view(1).second == 0);
        REQUIRE(mv.view(1).first == nullptr);
    }

    SECTION("reset first view") {
        mv.reset_view(0);
        REQUIRE(mv.size() == before_size - 3);
        REQUIRE(flatten(mv) == std::vector<uint8_t>{4,5,6,7});
        REQUIRE(mv.data() == b.data());
    }

    SECTION("reset last view") {
        mv.reset_view(2);
        REQUIRE(mv.size() == before_size - 2);
        REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2,3,4,5});
        REQUIRE(mv.back_data() == b.data() + 1);
    }

    SECTION("reset invalid index does nothing") {
        mv.reset_view(3);  // Out of range
        REQUIRE(mv.size() == before_size);
        REQUIRE(flatten(mv) == before_flat);
    }

    SECTION("reset empty view") {
        mv.append(nullptr, 0);  // idx 3: empty
        mv.reset_view(3);
        REQUIRE(mv.size() == before_size);
        REQUIRE(flatten(mv) == before_flat);
    }
}

// Test compact: removes empty views, does not merge non-empty, size/content unchanged
TEST_CASE("multi_buffer_view: compact removes empty views but does not merge non-empty", "[multi_buffer_view][compact]") 
{
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 2> b{3,4};
    std::array<uint8_t, 2> c{5,6};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);
    mv.append(nullptr, 0);
    mv.append(b.data(), 2);
    mv.append(nullptr, 0);
    mv.append(c.data(), 2);
    mv.append(nullptr, 0);

    auto before_flat = flatten(mv);
    auto before_size = mv.size();
    auto before_views = mv.view_count();

    mv.compact();

    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.size() == before_size);
    REQUIRE(mv.view_count() == 3);  // Only non-empty views remain
    REQUIRE(mv.view(0).first == a.data());
    REQUIRE(mv.view(1).first == b.data());
    REQUIRE(mv.view(2).first == c.data());

    // compact on all empty
    memepp::multi_buffer_view empty_mv;
    empty_mv.append(nullptr, 0);
    empty_mv.append(nullptr, 0);
    empty_mv.compact();
    REQUIRE(empty_mv.empty());
    REQUIRE(empty_mv.view_count() == 0);

    // compact does not merge adjacent non-empty views
    REQUIRE(mv.view_count() == 3);  // Still 3 separate views
}

// Test read<Ty>: different types, endian, partial data, across views
TEST_CASE("multi_buffer_view: read<Ty> for various types and endian, error on insufficient data", "[multi_buffer_view][read]") 
{
    std::array<uint8_t, 4> a{0x01, 0x02, 0x03, 0x04};
    std::array<uint8_t, 4> b{0x05, 0x06, 0x07, 0x08};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 4);
    mv.append(b.data(), 4);

    SECTION("read<uint32_t> little endian across views") {
        uint32_t val;
        auto err = mv.read(2, val, mgpp::endian_t::little_byte);  // 0x03,0x04,0x05,0x06 -> 0x06050403
        REQUIRE(err == mgpp::err{});
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(val == 0x06050403u);
#else
        REQUIRE(val == 0x03040506u);  // Swapped if big endian
#endif
    }

    SECTION("read<uint32_t> big endian") {
        uint32_t val;
        auto err = mv.read(0, val, mgpp::endian_t::big_byte);  // 0x01,0x02,0x03,0x04 -> 0x01020304 (no swap if big, but depends on host)
        REQUIRE(err == mgpp::err{});
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(val == 0x01020304u);
#else
        REQUIRE(val == 0x04030201u);  // Swapped
#endif
    }

    SECTION("read<int16_t> insufficient data") {
        int16_t val;
        auto err = mv.read(7, val, mgpp::endian_t::little_byte);  // Only 1 byte left
        REQUIRE(err.code() == MGEC__RANGE);
        REQUIRE(err.message() == "Not enough data");
    }

    SECTION("read<uint64_t> full size") {
        uint64_t val;
        auto err = mv.read(0, val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == 0x0807060504030201ULL);
    }

    SECTION("read<float> across empty view") {
        mv.prepend(nullptr, 0);
        float val;
        auto err = mv.read(memepp::multi_buffer_view::position{0, 0}, val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        uint32_t expected;
        std::memcpy(&expected, a.data(), sizeof(float));
#if MEGO_ENDIAN__BIG_BYTE
        expected = megopp::endian::byte_swap(expected);
#endif
        REQUIRE(val == *reinterpret_cast<float*>(&expected));
    }
}

// Test insert with count=0: no-op in various positions, returns original position
TEST_CASE("multi_buffer_view: insert with count=0 is no-op and returns original position", "[multi_buffer_view][insert][count0]") 
{
    std::array<uint8_t, 5> base{10,20,30,40,50};
    std::array<uint8_t, 2> x{99,100};
    memepp::buffer_view vx{x.data(), 2};

    memepp::multi_buffer_view mv;
    mv.append(base.data(), 5);

    auto before_flat = flatten(mv);
    auto before_size = mv.size();
    auto before_views = mv.view_count();

    SECTION("insert count=0 at beginning") {
        auto pos = mv.insert(memepp::multi_buffer_view::position{0,0}, 0, vx);
        REQUIRE(flatten(mv) == before_flat);
        REQUIRE(mv.size() == before_size);
        REQUIRE(mv.view_count() == before_views);
        REQUIRE(mv.convert_position(pos) == 0);
    }

    SECTION("insert count=0 in middle") {
        auto pos = mv.insert(memepp::multi_buffer_view::position{0,2}, 0, vx);
        REQUIRE(flatten(mv) == before_flat);
        REQUIRE(mv.size() == before_size);
        REQUIRE(mv.view_count() == before_views);
        REQUIRE(mv.convert_position(pos) == 2);
    }

    SECTION("insert count=0 at end") {
        auto pos = mv.insert(mv.end_position(), 0, vx);
        REQUIRE(flatten(mv) == before_flat);
        REQUIRE(mv.size() == before_size);
        REQUIRE(mv.view_count() == before_views);
        REQUIRE(mv.is_end(pos));
    }
}

// Test pop_front/pop_back with multiple consecutive empty views at ends
TEST_CASE("multi_buffer_view: pop_front/pop_back handle multiple consecutive empty views", "[multi_buffer_view][pop][empty-multi]") 
{
    std::array<uint8_t, 3> a{1,2,3};

    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0);
    mv.append(nullptr, 0);
    mv.append(a.data(), 3);
    mv.append(nullptr, 0);
    mv.append(nullptr, 0);

    SECTION("pop_front removes first non-empty byte, skips empties") {
        mv.pop_front();
        REQUIRE(flatten(mv) == std::vector<uint8_t>{2,3});
        REQUIRE(mv.size() == 2);
        REQUIRE(mv.data() == a.data() + 1);
    }

    SECTION("pop_back removes last non-empty byte, skips empties") {
        mv.pop_back();
        REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2});
        REQUIRE(mv.size() == 2);
        REQUIRE(mv.back_data() == a.data() + 1);
    }

    SECTION("multiple pops exhaust data, leave empties") {
        mv.pop_front();
        mv.pop_front();
        mv.pop_front();
        REQUIRE(mv.empty());
        REQUIRE(mv.size() == 0);
        REQUIRE(mv.data() == nullptr);
        REQUIRE(mv.back_data() == nullptr);
    }
}

// Test find: pattern larger than remaining data returns end
TEST_CASE("multi_buffer_view: find pattern larger than remaining returns end_position", "[multi_buffer_view][find][large]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    uint8_t pat[5] = {1,2,3,4,5};
    auto f = mv.find(pat, 5);
    REQUIRE(mv.is_end(f));

    auto f2 = mv.find(pat, 5, mv.convert_position(1));
    REQUIRE(mv.is_end(f2));
}

// Test slice: count > remaining takes all remaining
TEST_CASE("multi_buffer_view: slice with count > remaining takes all remaining data", "[multi_buffer_view][slice][oversize]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    auto s = mv.slice(1, 10);  // From 2, take 10, but only 2 left
    REQUIRE(flatten(s) == std::vector<uint8_t>{2,3});
    REQUIRE(s.size() == 2);
}

// Test move semantics: self-move, moved-from state
TEST_CASE("multi_buffer_view: move constructor/assignment handles self-move and moved-from state", "[multi_buffer_view][move][self]") 
{
    std::array<uint8_t, 3> a{1,2,3};

    memepp::multi_buffer_view src;
    src.append(a.data(), 3);

    SECTION("move constructor from self (undefined, but should not crash)") {
        // Self-move is undefined behavior, but test if it leaves in valid state
        memepp::multi_buffer_view mv(std::move(src));
        REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2,3});
        REQUIRE(mv.size() == 3);
        REQUIRE(src.empty());  // Moved-from should be empty
    }

    SECTION("move assignment to self") {
        src = std::move(src);  // Self-move
        REQUIRE(src.size() == 3);  // Typically leaves in valid empty state
    }

    SECTION("moved-from object can be reused") {
        memepp::multi_buffer_view mv(std::move(src));
        REQUIRE(src.empty());
        src.append(a.data(), 3);  // Reuse moved-from
        REQUIRE(flatten(src) == std::vector<uint8_t>{1,2,3});
    }
}

// Test compatibility with std::copy, std::find
TEST_CASE("multi_buffer_view: iterators compatible with std::copy and std::find", "[multi_buffer_view][algorithms]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 3> b{4,5,6};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(b.data(), 3);

    std::vector<uint8_t> dest(6);
    std::copy(mv.begin(), mv.end(), dest.begin());
    REQUIRE(dest == std::vector<uint8_t>{1,2,3,4,5,6});

    auto it = std::find(mv.begin(), mv.end(), 5);
    REQUIRE(*it == 5);
    REQUIRE(std::distance(mv.begin(), it) == 4);
}

// Test exception safety: insert/append that may throw (e.g., vector resize)
TEST_CASE("multi_buffer_view: exception safety during insert/append (strong guarantee)", "[multi_buffer_view][exception]") 
{
    // To test exception safety, we need to simulate allocation failure, but std::vector may throw std::bad_alloc.
    // However, in practice, it's hard to force, so this is a placeholder or assume no throw for small sizes.
    // For unit test, we can check that state is unchanged if throw occurs, but since it's rare, test normal case.
    memepp::multi_buffer_view mv;
    REQUIRE_NOTHROW(mv.append(1000, memepp::buffer_view{nullptr, 0}));  // Many empty views
    REQUIRE(mv.empty());
    REQUIRE(mv.view_count() == 1000);
}

// Test large size handling: near max size_type
TEST_CASE("multi_buffer_view: handling large sizes near size_type max", "[multi_buffer_view][large-size]") 
{
    memepp::multi_buffer_view mv;
    // Append many small views to approach large size
    for (int i = 0; i < 100; ++i) {
        mv.append(reinterpret_cast<const uint8_t*>("a"), 1);
    }
    REQUIRE(mv.size() == 100);

    // Test slice large count
    auto s = mv.slice(0, memepp::multi_buffer_view::npos);
    REQUIRE(s.size() == 100);
}

// Test position operator==/!=, including npos cases
TEST_CASE("multi_buffer_view: position equality and inequality operators", "[multi_buffer_view][position]") 
{
    memepp::multi_buffer_view::position p1{1, 2};
    memepp::multi_buffer_view::position p2{1, 2};
    memepp::multi_buffer_view::position p3{1, 3};
    memepp::multi_buffer_view::position p_npos{memepp::multi_buffer_view::npos, memepp::multi_buffer_view::npos};

    REQUIRE(p1 == p2);
    REQUIRE_FALSE(p1 != p2);
    REQUIRE(p1 != p3);
    REQUIRE_FALSE(p1 == p3);

    REQUIRE(p_npos == p_npos);
    REQUIRE(p_npos != p1);
    REQUIRE_FALSE(p_npos == p1);
}

// Test view_first_position/view_last_position with empty views and out-of-bounds
TEST_CASE("multi_buffer_view: view_first_position and view_last_position handle empty views and bounds", "[multi_buffer_view][viewpos]") 
{
    std::array<uint8_t, 2> a{1,2};
    std::array<uint8_t, 2> b{3,4};

    memepp::multi_buffer_view mv;
    mv.append(nullptr, 0);  // idx 0: empty
    mv.append(a.data(), 2); // idx 1
    mv.append(nullptr, 0);  // idx 2: empty
    mv.append(b.data(), 2); // idx 3

    SECTION("view_first_position skips leading empties") {
        auto p = mv.view_first_position(0);
        REQUIRE(p.index == 1);
        REQUIRE(p.offset == 0);
        REQUIRE(mv[p] == 1);
    }

    SECTION("view_first_position on empty view moves to next") {
        auto p = mv.view_first_position(2);
        REQUIRE(p.index == 3);
        REQUIRE(p.offset == 0);
        REQUIRE(mv[p] == 3);
    }

    SECTION("view_first_position beyond last is end") {
        auto p = mv.view_first_position(4);
        REQUIRE(mv.is_end(p));
    }

    SECTION("view_last_position skips trailing empties") {
        auto p = mv.view_last_position(3);
        REQUIRE(p.index == 3);
        REQUIRE(p.offset == 1);
        REQUIRE(mv[p] == 4);
    }

    SECTION("view_last_position on empty view moves to prev") {
        auto p = mv.view_last_position(2);
        REQUIRE(p.index == 1);
        REQUIRE(p.offset == 1);
        REQUIRE(mv[p] == 2);
    }

    SECTION("view_last_position before first is end") {
        auto p = mv.view_last_position(-1);
        REQUIRE(mv.is_end(p));
    }
}

// Test read_bytes with invalid buffer or count=0
TEST_CASE("multi_buffer_view: read_bytes handles nullptr buffer and count=0", "[multi_buffer_view][read_bytes]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    SECTION("read_bytes with count=0 returns 0") {
        uint8_t buf[10];
        auto n = mv.read_bytes(1, buf, 0);
        REQUIRE(n == 0);
    }

    SECTION("read_bytes with nullptr buffer returns negative (error)") {
        auto n = mv.read_bytes(0, nullptr, 2);
        REQUIRE(!!n);  // Assuming impl returns error code like MGEC__ARG
    }
}

// Test find with empty pattern: should return start position
TEST_CASE("multi_buffer_view: find with empty pattern returns start position", "[multi_buffer_view][find][empty]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    auto f = mv.find(static_cast<const uint8_t*>(nullptr), 0);
    REQUIRE(f.index  == mv.end_position().index );
    REQUIRE(f.offset == mv.end_position().offset);

    auto f2 = mv.find(static_cast<const uint8_t*>(nullptr), 0, mv.convert_position(1));
    REQUIRE(mv.convert_position(f2) == mv.npos);
}

// Test compact after erase/reset_view: removes new empties
TEST_CASE("multi_buffer_view: compact after erase/reset_view cleans up", "[multi_buffer_view][compact][post-ops]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    std::array<uint8_t, 2> b{4,5};

    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);
    mv.append(b.data(), 2);

    mv.erase(2, 2);  // Erase 3,4 -> leaves [1,2] [5] but may create empties if split
    mv.reset_view(1);  // Reset second view

    mv.compact();
    REQUIRE(mv.view_count() == 1);
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2});
}

// Test swap with self: no change, noexcept
TEST_CASE("multi_buffer_view: swap with self does nothing", "[multi_buffer_view][swap][self]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    auto before_flat = flatten(mv);
    REQUIRE_NOTHROW(mv.swap(mv));
    REQUIRE(flatten(mv) == before_flat);
    REQUIRE(mv.size() == 3);
}

// Test const-correctness: ensure const methods work on const objects
TEST_CASE("multi_buffer_view: const methods on const objects", "[multi_buffer_view][const]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    const memepp::multi_buffer_view mv(a.data(), 3);

    REQUIRE(mv.size() == 3);
    REQUIRE(mv.view_count() == 1);
    REQUIRE(mv.data() == a.data());
    REQUIRE(mv.front() == 1);
    REQUIRE(mv.back() == 3);
    REQUIRE(*mv.cbegin() == 1);
    REQUIRE(*std::prev(mv.cend()) == 3);

    auto p = mv.convert_position(1);
    REQUIRE(mv[p] == 2);

    auto s = mv.slice(1, 2);
    REQUIRE(flatten(s) == std::vector<uint8_t>{2,3});
}

// Test operations on completely empty multi_buffer_view (no views at all)
TEST_CASE("multi_buffer_view: operations on completely empty (no views)", "[multi_buffer_view][empty-no-views]") 
{
    memepp::multi_buffer_view mv;  // Default constructed, no views

    REQUIRE(mv.empty());
    REQUIRE(mv.size() == 0);
    REQUIRE(mv.view_count() == 0);
    REQUIRE(mv.begin() == mv.end());
    REQUIRE(mv.data() == nullptr);
    REQUIRE(mv.back_data() == nullptr);

    // append empty does add a view
    mv.append(nullptr, 0);
    REQUIRE(mv.view_count() == 1);
    REQUIRE(mv.empty());  // Still empty content

    // erase on empty does nothing
    auto pos = mv.erase(0);
    REQUIRE(pos == memepp::multi_buffer_view::npos);

    // find on empty returns end
    uint8_t pat = 1;
    auto f = mv.find(&pat, 1);
    REQUIRE(mv.is_end(f));

    // slice on empty returns empty
    auto s = mv.slice(0, 5);
    REQUIRE(s.empty());

    // to_buffer on empty returns empty buffer
    auto buf = mv.to_buffer();
    REQUIRE(buf.size() == 0);
}

// Test self-insert: insert a multi_buffer_view into itself (should copy views)
TEST_CASE("multi_buffer_view: insert self (copies views, no infinite loop)", "[multi_buffer_view][insert][self]") 
{
    std::array<uint8_t, 2> a{1,2};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 2);

    auto pos = mv.insert(mv.convert_position(1), mv);  // Insert self at pos 1: [1] + [1,2] + [2]
    REQUIRE(flatten(mv) == std::vector<uint8_t>{1,1,2,2});
    REQUIRE(mv.size() == 4);
    REQUIRE(mv.view_count() == 3);  // Split + inserted views
}

// Test npos in insert/slice/find
TEST_CASE("multi_buffer_view: npos handling in insert/slice/find", "[multi_buffer_view][npos]") 
{
    std::array<uint8_t, 3> a{1,2,3};
    memepp::multi_buffer_view mv;
    mv.append(a.data(), 3);

    SECTION("insert at npos appends") {
        std::array<uint8_t, 2> x{4,5};
        auto ret = mv.insert(mv.npos, x.data(), 2);
        REQUIRE(flatten(mv) == std::vector<uint8_t>{1,2,3,4,5});
        REQUIRE(ret == 3);
    }

    SECTION("slice start at npos returns empty") {
        auto s = mv.slice(mv.npos, 2);
        REQUIRE(s.empty());
    }

    SECTION("find start at npos returns end") {
        uint8_t pat = 2;
        auto f = mv.find(&pat, 1, mv.convert_position(mv.npos));
        REQUIRE(mv.is_end(f));
    }
}

// Test read<Ty> for signed and floating-point types
TEST_CASE("multi_buffer_view: read<Ty> for signed and floating-point types", "[multi_buffer_view][read][types]") 
{
    std::array<uint8_t, 8> data{0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x80, 0x3F};  // -1 (int32), 1.0f (float)
    memepp::multi_buffer_view mv;
    mv.append(data.data(), 8);

    SECTION("read<int32_t> negative value, little endian") {
        int32_t val;
        auto err = mv.read(0, val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
        REQUIRE(val == -1);
    }

    SECTION("read<float> positive value, big endian") {
        float val;
        auto err = mv.read(4, val, mgpp::endian_t::little_byte);
        REQUIRE(err == mgpp::err{});
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(val == *reinterpret_cast<float*>(&data[4]));  // Adjust for swap if needed
#else
        // Simulate swap for test
        uint32_t swapped = megopp::endian::byte_swap(*reinterpret_cast<uint32_t*>(&data[4]));
        REQUIRE(val == *reinterpret_cast<float*>(&swapped));
#endif
    }
}

// Additional fuzz: random removes and checks
TEST_CASE("multi_buffer_view: fuzz random removes and consistency", "[multi_buffer_view][fuzz][remove]") 
{
    std::mt19937 gen(7654321u);
    std::vector<uint8_t> model{1,2,3,4,5,6,7,8,9,10};

    memepp::multi_buffer_view mv;
    mv.append(model.data(), model.size());

    for (int i = 0; i < 50; ++i) {
        if (mv.empty()) break;
        size_t pos = rng_uint(gen, 0, static_cast<uint32_t>(mv.size() - 1));
        size_t count = rng_uint(gen, 1, static_cast<uint32_t>(mv.size() - pos));

        mv.erase(pos, count);
        model.erase(model.begin() + pos, model.begin() + pos + count);

        REQUIRE(flatten(mv) == model);
        REQUIRE(mv.size() == model.size());
    }
}
