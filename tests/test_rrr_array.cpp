#include "catch.hpp"
#include <notima/rrr_array.hpp>
#include <random>
#include <set>

#include <iostream>
#include <notima/internal/stats.hpp>
#include <notima/wordy.hpp>

namespace // anonymous
{
    static std::string bin(uint64_t x)
    {
        std::string r;
        do
        {
            r.push_back("01"[x & 1]);
            x >>= 1;
        }
        while (x > 0);
        return std::string(r.rbegin(), r.rend());
    }

    template <size_t B>
    void make_items(size_t p_n, std::vector<uint64_t>& p_items)
    {
        uint64_t x = (1ULL << p_n) - 1;
        for (uint64_t i = 0; i < notima::detail::binom[B][p_n]; ++i)
        {
            REQUIRE(notima::wordy::popcount(x) == p_n);
            p_items.push_back(x);
            uint64_t t = (x | (x - 1)) + 1;
            uint64_t w = t | ((((t & -t) / (x & -x)) >> 1) - 1);
            REQUIRE(notima::wordy::popcount(w) == p_n);
            REQUIRE(w > x);
            x = w;
        }
    }

    template <size_t B, size_t N>
    void make_some_items(size_t p_seed, size_t p_j, std::vector<uint64_t>& p_items)
    {
        std::mt19937 rng(p_seed);

        std::vector<size_t> pos;
        for (size_t i = 0; i < B; ++i)
        {
            pos.push_back(i);
        }

        std::set<uint64_t> tmp;
        while (tmp.size() < p_j)
        {
            uint64_t x = 0;
            std::shuffle(pos.begin(), pos.end(), rng);
            for (size_t i = 0; i < N; ++i)
            {
                x |= 1ULL << pos[i];
            }
            tmp.insert(x);
        }
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

    template <size_t B, size_t N>
    void block_ordinal_test()
    {
        std::vector<uint64_t> items;
        make_items<B>(N, items);
        notima::detail::block_ordinal<B> X(N);

        for (size_t i = 0; i < items.size(); ++i)
        {
            REQUIRE(X.select(i) == items[i]);
        }

        for (size_t i = 0; i <= (1ULL << B); ++i)
        {
            size_t r = slow_rank(items, i);
            REQUIRE(X.rank(i) == r);
        }
    }

    template <size_t B, size_t N>
    void large_block_ordinal_test()
    {
        constexpr size_t J = 1001;
        std::vector<uint64_t> items;
        make_some_items<B,N>(19, J, items);
        notima::detail::block_ordinal<B> X(N);

        for (size_t i = 0; i < items.size(); ++i)
        {
            uint64_t x = items[i];
            REQUIRE(notima::wordy::popcount(x) == N);
            size_t r = X.rank(items[i]);
            REQUIRE(r < X.count());
            size_t y = X.select(r);
            REQUIRE(notima::wordy::popcount(y) == N);
            REQUIRE(x == y);
        }
    }
}
// namespace anonymous

TEST_CASE("Test block 4 ordinals", "[rrr-array-test]")
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

TEST_CASE("Test block 9 ordinals", "[rrr-array-test]")
{
    constexpr size_t B = 9;

    block_ordinal_test<B,3>();
    block_ordinal_test<B,5>();
}

TEST_CASE("Test block 63 ordinals", "[rrr-array-test]")
{
    constexpr size_t B = 63;

    large_block_ordinal_test<B,3>();
    large_block_ordinal_test<B,5>();
    large_block_ordinal_test<B,15>();
    large_block_ordinal_test<B,31>();
    large_block_ordinal_test<B,48>();
}
