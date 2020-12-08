#include "catch.hpp"
#include <kmers.hpp>
#include <sstream>

namespace // anonymous
{
    std::string seq1("GCCTGCAGAACAGAGCATTTGATGACGAATTAGGGCTTGGTGGCTCATGCCCTCCTATGAGGGAAAGTGATACTAGACAAGAAAACTTGAAAACCAAGGCTCTCGTTTCTAACAGTTCTTTGCATTGGATACCCATCCCATCGAATGATGAGGT");
    std::string seq2("TGATGATGCTGATAATGCCAGTAAACTAGCTGCAATGCTAAATACCTGTTCCTTTCAGAAACCAGAACAACTACGACAACAAAAATCAGTTTTTGAGATATGCCCATCTCCTGCAGAAAATAACATCCAGGGAACCACAAAGCTAGCGTCTGGT");
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
