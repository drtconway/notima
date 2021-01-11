#include "catch.hpp"
#include <notima/rrr_array.hpp>
#include <random>
#include <set>

#include <iostream>
#include <notima/internal/stats.hpp>

namespace // anonymous
{
    template <size_t B>
    void make_items(uint64_t p_seed, size_t p_n, std::vector<uint64_t>& p_items)
    {
        constexpr uint64_t M = (1ULL << B) - 1;

        std::mt19937 rng(p_seed);
        std::uniform_real_distribution<> U(0, M);

        std::set<uint64_t> tmp;
        while (tmp.size() < p_n)
        {
            tmp.insert(U(rng));
        }
        p_items.clear();
        p_items.insert(p_items.end(), tmp.begin(), tmp.end());
    }

    uint64_t slow_rank(const std::vector<uint64_t>& p_items, uint64_t p_x)
    {
        return std::lower_bound(p_items.begin(), p_items.end(), p_x) - p_items.begin();
    }

    std::string hex(uint64_t x)
    {
        std::string rev;
        do
        {
            rev.push_back("0123456789abcdef"[x & 0xf]);
            x >>= 4;
        }
        while (x > 0);

        std::string res("0x");
        res.insert(res.end(), rev.rbegin(), rev.rend());
        return res;
    }
}
// namespace anonymous

TEST_CASE("Test block 5 ordinals", "[rrr-array-test]")
{
    constexpr size_t B = 4;

    if (1)
    {
        notima::detail::block_ordinal<B> X(0);
        REQUIRE(X.size() == (1ULL << B));
        REQUIRE(X.count() == 1);
        REQUIRE(X.select(0) == 0);
        REQUIRE(X.rank(0x0) == 0);
        REQUIRE(X.rank(0x3) == 1);
        REQUIRE(X.rank(0x5) == 1);
    }
    if (1)
    {
        notima::detail::block_ordinal<B> X(4);
        REQUIRE(X.size() == (1ULL << B));
        REQUIRE(X.count() == 1);
        REQUIRE(X.select(0) == 0xf);
        REQUIRE(X.rank(0x0) == 0);
        REQUIRE(X.rank(0x3) == 0);
        REQUIRE(X.rank(0x5) == 0);
        REQUIRE(X.rank(0x7) == 0);
        REQUIRE(X.rank(0x9) == 0);
        REQUIRE(X.rank(0xb) == 0);
        REQUIRE(X.rank(0xd) == 0);
        REQUIRE(X.rank(0xf) == 0);
        REQUIRE(X.rank(0x10) == 1);
    }
    if (1)
    {
        notima::detail::block_ordinal<B> X(1);
        REQUIRE(X.size() == (1ULL << B));
        REQUIRE(X.count() == B);
        REQUIRE(X.select(0) == 0x1);
        REQUIRE(X.select(1) == 0x2);
        REQUIRE(X.select(2) == 0x4);
        REQUIRE(X.select(3) == 0x8);
        REQUIRE(X.rank(0x0) == 0);
        REQUIRE(X.rank(0x1) == 0);
        REQUIRE(X.rank(0x2) == 1);
        REQUIRE(X.rank(0x3) == 2);
        REQUIRE(X.rank(0x5) == 3);
        REQUIRE(X.rank(0x7) == 3);
        REQUIRE(X.rank(0x9) == 4);
        REQUIRE(X.rank(0x0) == 0);
        REQUIRE(X.rank(0x1) == 0);
    }
    if (1)
    {
        notima::detail::block_ordinal<B> X(2);
        REQUIRE(X.size() == (1ULL << B));
        REQUIRE(X.count() == (B*(B-1)/2));
        REQUIRE(X.select(0) == 0x3);
        REQUIRE(X.select(1) == 0x5);
        REQUIRE(X.select(2) == 0x6);
        REQUIRE(X.select(3) == 0x9);
        REQUIRE(X.select(4) == 0xa);
        REQUIRE(X.select(5) == 0xc);
        REQUIRE(X.rank(0x0) == 0);
        REQUIRE(X.rank(0x1) == 0);
        REQUIRE(X.rank(0x3) == 0);
        REQUIRE(X.rank(0x4) == 1);
        REQUIRE(X.rank(0x5) == 1);
        REQUIRE(X.rank(0x6) == 2);
        REQUIRE(X.rank(0x7) == 3);
        REQUIRE(X.rank(0x8) == 3);
        REQUIRE(X.rank(0x9) == 3);
        REQUIRE(X.rank(0xa) == 4);
        REQUIRE(X.rank(0xb) == 5);
        REQUIRE(X.rank(0xc) == 5);
        REQUIRE(X.rank(0xd) == 6);
        REQUIRE(X.rank(0xe) == 6);
        REQUIRE(X.rank(0xf) == 6);
    }
}
