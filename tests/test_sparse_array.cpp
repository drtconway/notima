#include "catch.hpp"
#include <notima/sparse_array.hpp>
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

TEST_CASE("Test small sparse array", "[sparse-array-test]")
{
    constexpr size_t B = 20;
    constexpr size_t W = (1ULL << B) - 1;
    constexpr size_t N = 20;
    constexpr size_t D = notima::compute_d<B, N>::D;
    constexpr uint64_t M = (1ULL << D) - 1;

    REQUIRE(D == 16);

    std::vector<uint64_t> items;
    make_items<B>(19, N, items);

    notima::sparse_array_d<D> a(B, N, items);

    for (size_t i = 0; i < N; ++i)
    {
        uint64_t x = items[i];
        size_t r = a.rank(x);
        uint64_t y = a.select(i);
        if (0)
        {
            uint64_t x_hi = x >> D;
            uint64_t x_lo = x & M;
            uint64_t y_hi = y >> D;
            uint64_t y_lo = y & M;
            std::cout << "i\t" << i << std::endl;
            std::cout << "x\t" << hex(x_hi) << '\t' << hex(x_lo) << std::endl;
            std::cout << "y\t" << hex(y_hi) << '\t' << hex(y_lo) << std::endl;
        }
        REQUIRE(r == i);
        REQUIRE(y == x);
    }

    for (uint64_t x = 0; x <= W+1; ++x)
    {
        uint64_t r = a.rank(x);
        uint64_t s = slow_rank(items, x);
        REQUIRE(r == s);
    }
}

TEST_CASE("Test medium sparse array", "[sparse-array-test]")
{
    constexpr size_t B = 32;
    constexpr size_t N = 10001;
    constexpr size_t D = notima::compute_d<B, N>::D;
    constexpr uint64_t M = (1ULL << D) - 1;

    REQUIRE(D == 19);

    std::vector<uint64_t> items;
    make_items<B>(19, N, items);

    notima::sparse_array_d<D> a(B, N, items);

    for (size_t i = 0; i < N; ++i)
    {
        uint64_t x = items[i];
        size_t r = a.rank(x);
        uint64_t y = a.select(i);
        //uint64_t x_hi = x >> D;
        //uint64_t x_lo = x & M;
        //uint64_t y_hi = y >> D;
        //uint64_t y_lo = y & M;
        //std::cout << "x\t" << hex(x_hi) << '\t' << hex(x_lo) << std::endl;
        //std::cout << "y\t" << hex(y_hi) << '\t' << hex(y_lo) << std::endl;
        REQUIRE(r == i);
        REQUIRE(y == x);
    }

    if (0)
    {
        std::cout << notima::internal::stats::gather(a) << std::endl;
    }
}

TEST_CASE("Test small sparse array with", "[sparse-array-test]")
{
    constexpr size_t B = 20;
    constexpr size_t W = (1ULL << B) - 1;
    constexpr size_t N = 1001;

    std::vector<uint64_t> items;
    make_items<B>(19, N, items);

    notima::sparse_array a(B, items);
    a.with([&](auto& arr){
        size_t n = arr.count();
        for (size_t i = 0; i < n; ++i)
        {
            uint64_t x = arr.select(i);
            size_t r = arr.rank(x);
            REQUIRE(r == i);
        }
    });
}

