
#include "catch.hpp"
#include <notima/wordy.hpp>

#include <iostream>

namespace // anonymous
{
    uint64_t slow_popcount(uint64_t x)
    {
        uint64_t c = 0;
        while (x > 0)
        {
            c += (x & 1);
            x >>= 1;
        }
        return c;
    }

    uint64_t slow_rank(uint64_t x, uint64_t i)
    {
        uint64_t r = 0;
        for (size_t j = 0; j < i; ++j)
        {
            r += x & 1;
            x >>= 1;
        }
        return r;
    }

    uint64_t slow_select(uint64_t x, uint64_t r)
    {
        for (size_t i = 0; i < 64; ++i)
        {
            if (x & 1)
            {
                if (r == 0)
                {
                    return i;
                }
                r -= 1;
            }
            x >>= 1;
        }
        return 0xFFFFFFFFFFFFFFFFULL;
    }
}
// namespace anonymous

TEST_CASE("Test broadword constants", "[wordy-test]")
{
    REQUIRE(notima::wordy::L<8> == 0x0101010101010101ULL);
    REQUIRE(notima::wordy::H<8> == 0x8080808080808080ULL);
}

TEST_CASE("Test broadword popcount", "[wordy-test]")
{
    {
        uint64_t x = 0;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
    {
        uint64_t x = 1;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
    {
        uint64_t x = 0x1010101010101010ULL;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
    {
        uint64_t x = 0xFFFFFFFFFFFFFFFFULL;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
    {
        uint64_t x = 0xc1053d30352646b9ULL;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
    {
        uint64_t x = 0xc09faf832822b22bULL;
        REQUIRE(notima::wordy::popcount(x) == slow_popcount(x));
    }
}
TEST_CASE("Test broadword rank", "[wordy-test]")
{
    {
        uint64_t x = 0;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
    {
        uint64_t x = 1;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
    {
        uint64_t x = 0x1010101010101010ULL;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
    {
        uint64_t x = 0xFFFFFFFFFFFFFFFFULL;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
    {
        uint64_t x = 0xc1053d30352646b9ULL;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
    {
        uint64_t x = 0xc09faf832822b22bULL;
        for (uint64_t i = 0; i <= 64; ++i)
        {
            REQUIRE(notima::wordy::rank(x, i) == slow_rank(x, i));
        }
    }
}

TEST_CASE("Test broadword select", "[wordy-test]")
{
    {
        uint64_t x = 0x1010101010101010ULL;
        uint64_t c = notima::wordy::popcount(x);
        for (uint64_t r = 0; r < c; ++r)
        {
            REQUIRE(notima::wordy::select(x, r) == slow_select(x, r));
            uint64_t i = notima::wordy::select(x, r);
            REQUIRE(notima::wordy::rank(x, i) == r);
        }
    }
    {
        uint64_t x = 0xFFFFFFFFFFFFFFFFULL;
        uint64_t c = notima::wordy::popcount(x);
        for (uint64_t r = 0; r < c; ++r)
        {
            REQUIRE(notima::wordy::select(x, r) == slow_select(x, r));
            uint64_t i = notima::wordy::select(x, r);
            REQUIRE(notima::wordy::rank(x, i) == r);
        }
    }
    {
        uint64_t x = 0xc1053d30352646b9ULL;
        uint64_t c = notima::wordy::popcount(x);
        for (uint64_t r = 0; r < c; ++r)
        {
            REQUIRE(notima::wordy::select(x, r) == slow_select(x, r));
            uint64_t i = notima::wordy::select(x, r);
            REQUIRE(notima::wordy::rank(x, i) == r);
        }
    }
    {
        uint64_t x = 0xc09faf832822b22bULL;
        uint64_t c = notima::wordy::popcount(x);
        for (uint64_t r = 0; r < c; ++r)
        {
            REQUIRE(notima::wordy::select(x, r) == slow_select(x, r));
            uint64_t i = notima::wordy::select(x, r);
            REQUIRE(notima::wordy::rank(x, i) == r);
        }
    }
}
