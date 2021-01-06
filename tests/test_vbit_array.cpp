#include "catch.hpp"
#include <notima/vbit_array.hpp>
#include <cmath>
#include <random>
#include <map>
#include <iostream>

#include <notima/internal/stats.hpp>
namespace // anonymous
{
    static size_t bit_width(uint64_t x)
    {
        size_t n = 0;
        while (x > 0)
        {
            n += 1;
            x >>= 1;
        }
        return n;
    }

    void make_items(uint64_t p_seed, size_t p_n,
                    std::vector<uint64_t>& p_items)
    {
        std::mt19937 rng(p_seed);
        std::uniform_real_distribution<double> U;
        std::map<size_t,size_t> H;
        for (size_t i = 0; i < p_n; ++i)
        {
            double u = U(rng);
            uint64_t x = 1024 * -std::log2(u);
            H[bit_width(x)] += 1;
            p_items.push_back(x);
        }

        if (0)
        {
            for (auto itr = H.begin(); itr != H.end(); ++itr)
            {
                std::cout << itr->first << '\t' << itr->second << std::endl;
            }
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

TEST_CASE("Test a vbit array", "[vbit-array-test]")
{
    constexpr size_t N = 200001;

    std::vector<uint64_t> items;
    make_items(191, N, items);
    REQUIRE(items.size() == N);

    notima::vbit_array A(items);

    REQUIRE(A.size() == N);

    for (size_t i = 0; i < N; ++i)
    {
        uint64_t x = A[i];
        REQUIRE(x == items[i]);
    }

    nlohmann::json j = notima::internal::stats::gather(A);
    uint64_t s = j["vbit"]["size"].get<uint64_t>();
    uint64_t m = j["vbit"]["memory"].get<uint64_t>();
    std::cout << s << '\t' << m << '\t' << (8.0 * m / s) << std::endl;
    //std::cout << notima::internal::stats::gather(A) << std::endl;
}
