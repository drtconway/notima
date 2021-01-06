#include "catch.hpp"
#include <notima/bit_array.hpp>
#include <random>
#include <iostream>

namespace // anonymous
{
    void make_items(uint64_t p_seed, size_t p_n,
                    std::vector<size_t>& p_sizes,
                    std::vector<uint64_t>& p_items)
    {
        std::mt19937 rng(p_seed);
        std::uniform_int_distribution<size_t> U(0, 63);
        for (size_t i = 0; i < p_n; ++i)
        {
            size_t b = U(rng);
            std::uniform_int_distribution<uint64_t> V(0, (1ULL << b) - 1);
            uint64_t x = V(rng);
            p_sizes.push_back(b);
            p_items.push_back(x);
        }
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

TEST_CASE("Test bit array [wide]", "[bit-array-test]")
{
    constexpr size_t N = 1001;

    std::vector<size_t> sizes;
    std::vector<uint64_t> items;
    make_items(19, N, sizes, items);
    REQUIRE(sizes.size() == N);
    REQUIRE(items.size() == N);

    notima::bit_array A;

    size_t t = 0;
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(A.size() == t);
        A.push_back(sizes[i], items[i]);
        t += sizes[i];
    }
    REQUIRE(A.size() == t);

    t = 0;
    for (size_t i = 0; i < N; ++i)
    {
        uint64_t x = A[{t, t+sizes[i]}];
        if (x != items[i])
        {
            std::cout << i
                << '\t' << t
                << '\t' << (t + sizes[i])
                << '\t' << sizes[i]
                << '\t' << hex(items[i])
                << '\t' << hex(x)
                << std::endl;
            for (size_t j = 0; j < A.words.size(); ++j)
            {
                std::cout << ' ' << hex(A.words[j]);
            }
            std::cout << std::endl;
        }
        REQUIRE(x == items[i]);
        t += sizes[i];
    }
}
