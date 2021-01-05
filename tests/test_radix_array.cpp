#include "catch.hpp"
#include <notima/radix_array.hpp>
#include <notima/subword_array.hpp>

#include <vector>
#include <iostream>
#include <notima/internal/stats.hpp>

namespace // anonymous
{
}
// namespace anonymous

TEST_CASE("Test array bits of vector", "[radix-array-test]")
{
    using without_type = std::vector<uint16_t>;
    using without_computed_type = notima::detail::array_value_bits<without_type>;
    REQUIRE(without_computed_type::value == 16);
}

TEST_CASE("Test array bits of subword array", "[radix-array-test]")
{
    using with_type = notima::subword_array<4,16>;
    using with_computed_type = notima::detail::array_value_bits<with_type>;
    REQUIRE(with_computed_type::value == 4);
}

TEST_CASE("Test 1 level radix array", "[radix-array-test]")
{
    using a1 = std::vector<uint16_t>;
    notima::radix_array<a1> R;

    REQUIRE(R.size() == 0);

    constexpr size_t N = 1001;
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(R.size() == i);
        R.push_back(i);
    }
    REQUIRE(R.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(R[i] == i);
    }
}

TEST_CASE("Test 2 level radix array", "[radix-array-test]")
{
    using a1 = std::vector<uint8_t>;
    notima::radix_array<a1,a1> R;

    REQUIRE(R.size() == 0);

    constexpr size_t N = 1001;
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(R.size() == i);
        R.push_back(i);
    }
    REQUIRE(R.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(R[i] == i);
    }
}

TEST_CASE("Test 3 level radix array", "[radix-array-test]")
{
    using a1 = std::vector<uint8_t>;
    using a2 = std::vector<uint16_t>;
    notima::radix_array<a1,a1,a2> R;

    REQUIRE(R.size() == 0);

    constexpr size_t N = 1001;
    for (size_t i = 0; i < N; ++i)
    {
        REQUIRE(R.size() == i);
        uint32_t x = 2381616247ULL * i + 2549443283ULL;
        R.push_back(x);
    }
    REQUIRE(R.size() == N);
    for (size_t i = 0; i < N; ++i)
    {
        uint32_t x = 2381616247ULL * i + 2549443283ULL;
        REQUIRE(R[i] == x);
    }

    if (0)
    {
        std::cout << notima::internal::stats::gather(R) << std::endl;
    }
}
