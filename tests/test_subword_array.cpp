#include "catch.hpp"
#include <notima/subword_array.hpp>

namespace // anonymous
{
    template <size_t B, size_t W>
    void do_test()
    {
        constexpr size_t V = W / B;
        constexpr uint64_t M = (1ULL << B) - 1;
        constexpr size_t N = 1001;

        notima::subword_array<B,W> A;

        REQUIRE(A.size() == 0);

        for (uint64_t i = 0; i < N; ++i)
        {
            REQUIRE(A.size() == i);
            
            uint64_t x = i & M;
            A.push_back(x);
        }

        REQUIRE(A.size() == N);
        REQUIRE(A.words.size() == ((N + V - 1) / V));

        for (uint64_t i = 0; i < N; ++i)
        {
            uint64_t x = i & M;
            REQUIRE(A[i] == x);
        }
    }
}
// namespace anonymous

TEST_CASE("Test simple subword array", "[subword-array-test]")
{
    constexpr size_t B = 4;
    constexpr size_t W = 16;
    constexpr size_t V = W / B;
    constexpr uint64_t M = (1ULL << B) - 1;
    constexpr size_t N = 1001;

    notima::subword_array<B,W> A;

    REQUIRE(A.size() == 0);

    for (uint64_t i = 0; i < N; ++i)
    {
        REQUIRE(A.size() == i);
        
        uint64_t x = i & M;
        A.push_back(x);
    }

    REQUIRE(A.size() == N);
    REQUIRE(A.words.size() == ((N + V - 1) / V));

    for (uint64_t i = 0; i < N; ++i)
    {
        uint64_t x = i & M;
        REQUIRE(A[i] == x);
    }
}

TEST_CASE("Test subword array sizes", "[subword-array-test]")
{
    do_test<1,8>();
    do_test<2,8>();
    do_test<4,8>();
    do_test<8,8>();
    do_test<1,16>();
    do_test<2,16>();
    do_test<4,16>();
    do_test<8,16>();
    do_test<16,16>();
    do_test<1,32>();
    do_test<2,32>();
    do_test<4,32>();
    do_test<8,32>();
    do_test<16,32>();
    do_test<32,32>();
    do_test<1,64>();
    do_test<2,64>();
    do_test<4,64>();
    do_test<8,64>();
    do_test<16,64>();
    do_test<32,64>();
}
