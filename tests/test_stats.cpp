#include "catch.hpp"
#include <notima/stats.hpp>

#include <random>

#include <iostream>

namespace // anonymous
{
}
// namespace anonymous

TEST_CASE("Test kolmogorov smirnov test - normal", "[stats-test]")
{
    std::mt19937 rng{17};

    const size_t N = 200;
    std::vector<double> xs;

    {
        std::normal_distribution<> D{0.0, 1.0};
        for (size_t i = 0; i < N; ++i)
        {
            xs.push_back(D(rng));
        }
    }

    notima::stats::empirical_distribution<> e(xs.begin(), xs.end());
    notima::stats::normal_distribution<> n(0.0, 1.0);

    double d = notima::stats::kolmogorov_smirnov(e, n);
    double v = d * std::sqrt(N);
    REQUIRE(v < 0.85);
}

TEST_CASE("Test kolmogorov smirnov test - uniform", "[stats-test]")
{
    std::mt19937 rng{17};

    const size_t N = 200;
    std::vector<double> xs;

    {
        std::uniform_real_distribution<> D{0.0, 1.0};
        for (size_t i = 0; i < N; ++i)
        {
            xs.push_back(D(rng));
        }
    }

    notima::stats::empirical_distribution<> e(xs.begin(), xs.end());
    notima::stats::uniform_distribution<> n(0.0, 1.0);

    double d = notima::stats::kolmogorov_smirnov(e, n);
    double v = d * std::sqrt(N);
    REQUIRE(v < 0.65);
}
