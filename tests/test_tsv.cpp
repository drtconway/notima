#include "catch.hpp"
#include <notima/tsv.hpp>
#include <sstream>

namespace // anonymous
{
    std::string tsv_str(
        "ABL1\tLeu\t9\tPro\t0.23\n"
        "ABL2\tTrp\t745\tGly\t0.84\n"
    );
}
// namespace anonymous

TEST_CASE("Test tsv parser 1", "[tsv-test]")
{
    using tbl_type = notima::tsv<std::string,std::string,uint32_t,std::string,float>;
    using tuple_type = tbl_type::tuple_type;

    std::istringstream inp(tsv_str);
    tbl_type tbl(inp);
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
