#include "catch.hpp"
#include <kmers.hpp>
#include <sstream>

#include <notima/stats.hpp>
#include <iostream>

namespace // anonymous
{
    std::string seq1("GCCTGCAGAACAGAGCATTTGATGACGAATTAGGGCTTGGTGGCTCATGCCCTCCTATGAGGGAAAGTGATACTAGACAAGAAAACTTGAAAACCAAGGCTCTCGTTTCTAACAGTTCTTTGCATTGGATACCCATCCCATCGAATGATGAGGT");
    std::string seq2("TGATGATGCTGATAATGCCAGTAAACTAGCTGCAATGCTAAATACCTGTTCCTTTCAGAAACCAGAACAACTACGACAACAAAAATCAGTTTTTGAGATATGCCCATCTCCTGCAGAAAATAACATCCAGGGAACCACAAAGCTAGCGTCTGGT");

    double ks_test(const std::vector<double>& p_samples, const double& p_min, const double& p_max)
    {
        const size_t n = p_samples.size();
        notima::stats::empirical_distribution<> e(p_samples.begin(), p_samples.end());
        notima::stats::uniform_distribution<> u(p_min, p_max);
        double d = notima::stats::kolmogorov_smirnov(e, u);
        return d * std::sqrt(n);
    }
}
// namespace anonymous

TEST_CASE("Test kmer extraction", "[kmers-test]")
{
    std::vector<notima::kmer> fwd;
    std::vector<notima::kmer> rev;
    size_t K = 8;
    size_t N = seq1.size() - K + 1;
    notima::kmers::make(K, seq1, fwd, rev);
    REQUIRE(fwd.size() == N);
    REQUIRE(rev.size() == N);
    std::vector<std::string> expected{
        "GCCTGCAG", "AACAGAGC", "ATTTGATG", "ACGAATTA",
        "GGGCTTGG", "TGGCTCAT", "GCCCTCCT", "ATGAGGGA",
        "AAGTGATA", "CTAGACAA", "GAAAACTT", "GAAAACCA",
        "AGGCTCTC", "GTTTCTAA", "CAGTTCTT", "TGCATTGG",
        "ATACCCAT", "CCCATCGA", "ATGATGAG"
    };
    for (size_t i = 0; i < expected.size(); ++i)
    {
        REQUIRE(notima::kmers::render(K, fwd[K*i]) == expected[i]);
    }
    for (size_t i = 0; i < fwd.size(); ++i)
    {
        notima::kmer x = fwd[i];
        notima::kmer xb = notima::kmers::rc(K, x);
        notima::kmer y = rev[i];
        notima::kmer yb = notima::kmers::rc(K, y);
        REQUIRE(x == yb);
        REQUIRE(xb == y);
    }
}

TEST_CASE("Test kmer hashing", "[kmers-test]")
{
    std::vector<notima::kmer> fwd;
    std::vector<notima::kmer> rev;
    size_t K = 8;
    size_t N = seq1.size() - K + 1;
    notima::kmers::make(K, seq1, fwd, rev);
    REQUIRE(fwd.size() == N);
    REQUIRE(rev.size() == N);
    std::vector<double> ps;
    std::vector<double> qs;
    for (size_t i = 0; i < fwd.size(); ++i)
    {
        ps.push_back(notima::kmers::unif(fwd[i], 19));
        qs.push_back(notima::kmers::unif(rev[i], 19));
    }
    REQUIRE(ks_test(ps, 0.0, 1.0) < 1.0);
    REQUIRE(ks_test(qs, 0.0, 1.0) < 1.0);
}
