#include "catch.hpp"
#include <notima/poppy.hpp>
#include <random>

namespace // anonymous
{
    uint64_t rnd_word(std::mt19937& p_rng, double p_density)
    {
        std::uniform_real_distribution<> U;

        uint64_t w = 0;
        for (size_t i = 0; i < 64; ++i)
        {
            bool bit = (U(p_rng) < p_density);
            w = (w << 1) | static_cast<uint64_t>(bit);
        }
        return w;
    }

    void make_words(uint64_t p_seed, double p_density, size_t p_n, std::vector<uint64_t>& p_words)
    {
        std::mt19937 rng(p_seed);
        p_words.reserve(p_n);
        for (size_t i = 0; i < p_n; ++i)
        {
            uint64_t w = rnd_word(rng, p_density);
            p_words.push_back(w);
        }
    }

    void scan_for_ones(const std::vector<uint64_t>& p_words, std::vector<uint64_t>& p_ones)
    {
        for (size_t i = 0; i < p_words.size(); ++i)
        {
            uint64_t w = p_words[i];
            for (size_t j = 0; j < 64; ++j)
            {
                uint64_t p = 64*i + j;
                if (w & 1)
                {
                    p_ones.push_back(p);
                }
                w >>= 1;
            }
        }
    }
}
// namespace anonymous

TEST_CASE("Test poppy mid-density", "[poppy-test]")
{
    constexpr size_t N = 48;
    std::vector<uint64_t> words;
    make_words(19, 0.4, N, words);

    std::vector<uint64_t> ones;
    scan_for_ones(words, ones);

    notima::poppy P(std::move(words));

    REQUIRE(P.size() == 64*N);
    REQUIRE(P.count() == ones.size());

    for (size_t i = 0; i < ones.size(); ++i)
    {
        uint64_t p = ones[i];
        uint64_t r = P.rank(p);
        uint64_t q = P.select(i);
        //std::cout << i << '\t' << r << '\t' << p << '\t' << q << std::endl;
        REQUIRE(r == i);
        REQUIRE(q == p);
    }
}

TEST_CASE("Test poppy low-density", "[poppy-test]")
{
    constexpr size_t N = 128;
    std::vector<uint64_t> words;
    make_words(19, 0.001, N, words);

    std::vector<uint64_t> ones;
    scan_for_ones(words, ones);

    notima::poppy P(std::move(words));

    REQUIRE(P.size() == 64*N);
    REQUIRE(P.count() == ones.size());

    std::cout << "P.count() = " << P.count() << std::endl;

    for (size_t i = 0; i < ones.size(); ++i)
    {
        uint64_t p = ones[i];
        uint64_t r = P.rank(p);
        uint64_t q = P.select(i);
        //std::cout << i << '\t' << r << '\t' << p << '\t' << q << std::endl;
        REQUIRE(r == i);
        REQUIRE(q == p);
    }
}
