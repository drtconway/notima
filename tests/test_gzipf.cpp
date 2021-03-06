
#include "catch.hpp"
#include <gzipf.hpp>

namespace // anonymous
{
    std::string fq_str(
        "@M00139:329:000000000-AN968:1:2105:16049:1343 1:N:0:GTTCGATTGT\n"
        "GCCTGCAGAACAGAGCATTTGATGACGAATTAGGGCTTGGTGGCTCATGCCCTCCTATGAGGGAAAGTGATACTAGACAAGAAAACTTGAAAACCAAGGCTCTCGTTTCTAACAGTTCTTTGCATTGGATACCCATCCCATCGAATGATGAGGT\n"
        "+\n"
        "ABBBBFFFFFFCGFGGGGFGGFDFHHCFAHHFHHHHHGGGGFGHGHFHHHHHHHGHHHHHHGGGHGHGFHGHHHHHGHHHHGGHHFHGGHHHHHGHHHFHHGHHGGGGHHHHHGHHHHHFHHHHHHHHHFGGFHHHHHHHHHFGHHHHHHHHHF\n"
        "@M00139:329:000000000-AN968:1:2105:15810:1362 1:N:0:GTTCGATTGT\n"
        "TGATGATGCTGATAATGCCAGTAAACTAGCTGCAATGCTAAATACCTGTTCCTTTCAGAAACCAGAACAACTACGACAACAAAAATCAGTTTTTGAGATATGCCCATCTCCTGCAGAAAATAACATCCAGGGAACCACAAAGCTAGCGTCTGGT\n"
        "+\n"
        "CCCCCFFFFFFBFGGGGGGGGGHHGHGHHHHHHHHHGHHHHHHHHHHHGHHHHHHHGHHHHHHHHHHHHGHHHHGGGGGHHHHHGHHHHGHHHGGGFEHHHHGHHHHHHHHHHHHHHHGGHHHHHHHGHHGGGHGGGGHHHGHHHHHGGGGGHG\n"
    );
}
// namespace anonymous

TEST_CASE("Test gzip round trip", "[gzipf-test]")
{
    {
        notima::gzip::output out("x.fq.gz");
        out.write(fq_str);
    }

    notima::gzip::input in("x.fq.gz");

    std::string all;
    std::string l;
    while (in.getline(l))
    {
        all += l;
        all.push_back('\n');
    }
    REQUIRE(all == fq_str);
}
