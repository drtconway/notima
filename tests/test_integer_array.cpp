#include "catch.hpp"

#include <notima/integer_array.hpp>

namespace // anonymous
{
    template <size_t B>
    void instantiate()
    {
        constexpr uint64_t M = (1ULL << B) - 1;

        notima::integer_array<B> a;

        constexpr size_t N = 1001;
        for (size_t i = 0; i < N; ++i)
        {
            REQUIRE(a.size() == i);
            uint64_t x = (2381616247ULL * i + 2549443283ULL) & M;
            a.push_back(x);
        }
        REQUIRE(a.size() == N);
        for (size_t i = 0; i < N; ++i)
        {
            uint64_t x = (2381616247ULL * i + 2549443283ULL) & M;
            REQUIRE(a[i] == x);
        }
    }
}
// namespace anonymous

TEST_CASE("Test array instantiation", "[integer-array-test]")
{
    instantiate<1>();
    instantiate<2>();
    instantiate<3>();
    instantiate<4>();
    instantiate<5>();
    instantiate<6>();
    instantiate<8>();
    instantiate<16>();
    instantiate<17>();
    instantiate<31>();
    instantiate<32>();
    instantiate<48>();
    instantiate<63>();
}
