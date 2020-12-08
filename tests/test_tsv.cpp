#include "catch.hpp"
#include <notima/tsv.hpp>
#include <sstream>
#include <iostream>

namespace // anonymous
{
    std::string tsv_str(
        "ABL1\tLeu\t9\tPro\t0.23\n"
        "ABL2\tTrp\t745\tGly\t0.84\n"
    );
    std::string tsv_str2(
        "bin\tname\tchrom\tstrand\ttxStart\ttxEnd\tcdsStart\tcdsEnd\texonCount\texonStarts\texonEnds\tscore\tname2\tcdsStartStat\tcdsEndStat\texonFrames\n"
        "0\tNM_001276352.2\tchr1\t-\t67092164\t67134970\t67093579\t67127240\t9\t67092164,67096251,67103237,67111576,67115351,67125751,67127165,67131141,67134929,\t67093604,67096321,67103382,67111644,67115464,67125909,67127257,67131227,67134970,\t0\tC1orf141\tcmpl\tcmpl\t2,1,0,1,2,0,0,-1,-1,\n"
        "0\tNM_001276351.2\tchr1\t-\t67092164\t67134970\t67093004\t67127240\t8\t67092164,67095234,67096251,67115351,67125751,67127165,67131141,67134929,\t67093604,67095421,67096321,67115464,67125909,67127257,67131227,67134970,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,2,0,0,-1,-1,\n"
        "0\tNR_075077.2\tchr1\t-\t67092164\t67134970\t67134970\t67134970\t10\t67092164,67096251,67103237,67111576,67113613,67115351,67125751,67127165,67131141,67134929,\t67093604,67096321,67103382,67111644,67113756,67115464,67125909,67127257,67131227,67134970,\t0\tC1orf141\tnone\tnone\t-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\n"
        "0\tXM_011541469.1\tchr1\t-\t67092175\t67109072\t67093004\t67103382\t5\t67092175,67095234,67096251,67103237,67109028,\t67093604,67095421,67096321,67103382,67109072,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,0,-1,\n"
        "0\tXM_011541467.1\tchr1\t-\t67092175\t67131183\t67093004\t67127240\t9\t67092175,67095234,67096251,67103237,67111576,67115351,67125751,67127165,67131141,\t67093604,67095421,67096321,67103343,67111644,67115464,67125909,67127257,67131183,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,0,1,2,0,0,-1,\n"
        "0\tXM_017001276.1\tchr1\t-\t67092175\t67131227\t67093004\t67127240\t9\t67092175,67095234,67096251,67103237,67111576,67115351,67125751,67127165,67131141,\t67093604,67095421,67096321,67103382,67111644,67115464,67125909,67127257,67131227,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,0,1,2,0,0,-1,\n"
        "0\tXM_011541465.2\tchr1\t-\t67092175\t67134962\t67093004\t67127240\t9\t67092175,67095234,67096251,67103237,67111576,67115351,67125751,67127165,67134929,\t67093604,67095421,67096321,67103382,67111644,67115464,67125909,67127257,67134962,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,0,1,2,0,0,-1,\n"
        "0\tXM_011541466.2\tchr1\t-\t67092175\t67141646\t67093004\t67127240\t9\t67092175,67095234,67096251,67103237,67111576,67115351,67125751,67127165,67141613,\t67093604,67095421,67096321,67103382,67111644,67115464,67125909,67127257,67141646,\t0\tC1orf141\tcmpl\tcmpl\t0,2,1,0,1,2,0,0,-1,\n"
        "0\tXM_017001277.1\tchr1\t-\t67093484\t67131227\t67093569\t67127240\t9\t67093484,67095353,67096251,67103237,67111576,67115351,67125751,67127165,67131141,\t67093604,67095421,67096321,67103382,67111644,67115464,67125909,67127257,67131227,\t0\tC1orf141\tcmpl\tcmpl\t1,2,1,0,1,2,0,0,-1,\n"
    );
}
// namespace anonymous

TEST_CASE("Test split 1", "[tsv-test]")
{
    std::string str = "";
    std::vector<std::string> vec;
    notima::split(str, ',', [&](auto beg, auto end) {
        vec.push_back(std::string(beg, end));
    });
    REQUIRE(vec.size() == 1);
    REQUIRE(vec[0] == "");
}

TEST_CASE("Test split 2", "[tsv-test]")
{
    std::string str = "123,456";
    std::vector<std::string> vec;
    notima::split(str, ',', [&](auto beg, auto end) {
        vec.push_back(std::string(beg, end));
    });
    REQUIRE(vec.size() == 2);
    REQUIRE(vec[0] == "123");
    REQUIRE(vec[1] == "456");
}

TEST_CASE("Test split 3", "[tsv-test]")
{
    std::string str = "123,,456";
    std::vector<std::string> vec;
    notima::split(str, ',', [&](auto beg, auto end) {
        vec.push_back(std::string(beg, end));
    });
    REQUIRE(vec.size() == 3);
    REQUIRE(vec[0] == "123");
    REQUIRE(vec[1] == "");
    REQUIRE(vec[2] == "456");
}

TEST_CASE("Test split 4", "[tsv-test]")
{
    std::string str = "123,,456,";
    std::vector<std::string> vec;
    notima::split(str, ',', [&](auto beg, auto end) {
        vec.push_back(std::string(beg, end));
    });
    REQUIRE(vec.size() == 4);
    REQUIRE(vec[0] == "123");
    REQUIRE(vec[1] == "");
    REQUIRE(vec[2] == "456");
    REQUIRE(vec[3] == "");
}

TEST_CASE("Test tsv parser 1", "[tsv-test]")
{
    using tbl_type = notima::tsv<std::string,std::string,uint32_t,std::string,float>;
    using tuple_type = tbl_type::tuple_type;

    std::istringstream inp(tsv_str);
    tbl_type::reader tbl(inp);
    tuple_type tup;
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<0>(tup) == "ABL1");
    REQUIRE(std::get<1>(tup) == "Leu");
    REQUIRE(std::get<2>(tup) == 9);
    REQUIRE(std::get<3>(tup) == "Pro");
    REQUIRE(std::get<4>(tup) == Approx(0.23));
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<0>(tup) == "ABL2");
    REQUIRE(std::get<1>(tup) == "Trp");
    REQUIRE(std::get<2>(tup) == 745);
    REQUIRE(std::get<3>(tup) == "Gly");
    REQUIRE(std::get<4>(tup) == Approx(0.84));
    REQUIRE(!tbl.next(tup));
}

TEST_CASE("Test tsv parser 2", "[tsv-test]")
{
    using tbl_type =
        notima::tsv<
            size_t,
            std::string,
            std::string,
            char,
            uint64_t,
            uint64_t,
            uint64_t,
            uint64_t,
            size_t,
            notima::rep<uint64_t,',',true>,
            notima::rep<uint64_t,',',true>,
            std::string,
            std::string,
            std::string,
            std::string,
            notima::rep<int,',',true>
        >;
    using tuple_type = tbl_type::tuple_type;

    std::istringstream inp(tsv_str2);
    tbl_type::reader tbl(inp, true);
    tuple_type tup;
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<0>(tup) == 0);
    REQUIRE(std::get<1>(tup) == "NM_001276352.2");
    REQUIRE(std::get<2>(tup) == "chr1");
    REQUIRE(std::get<3>(tup) == '-');
    REQUIRE(std::get<4>(tup) == 67092164);
    REQUIRE(std::get<5>(tup) == 67134970);
    REQUIRE(std::get<6>(tup) == 67093579);
    REQUIRE(std::get<7>(tup) == 67127240);
    REQUIRE(std::get<8>(tup) == 9);
    REQUIRE(std::get<9>(tup) == std::vector<uint64_t>({67092164,67096251,67103237,67111576,67115351,67125751,67127165,67131141,67134929}));
    REQUIRE(std::get<10>(tup) == std::vector<uint64_t>({67093604,67096321,67103382,67111644,67115464,67125909,67127257,67131227,67134970}));
    REQUIRE(std::get<11>(tup) == "0");
    REQUIRE(std::get<12>(tup) == "C1orf141");
    REQUIRE(std::get<13>(tup) == "cmpl");
    REQUIRE(std::get<14>(tup) == "cmpl");
    REQUIRE(std::get<15>(tup) == std::vector<int>({2,1,0,1,2,0,0,-1,-1}));
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "NM_001276351.2");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "NR_075077.2");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_011541469.1");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_011541467.1");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_017001276.1");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_011541465.2");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_011541466.2");
    REQUIRE(tbl.next(tup));
    REQUIRE(std::get<1>(tup) == "XM_017001277.1");
    REQUIRE(!tbl.next(tup));
}
