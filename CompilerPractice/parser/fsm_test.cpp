#include "fsm.h"
#include "grammar.h"
#include "catch2/catch.hpp"

//Expression Grammar; is not LR(0) but is SLR(1)
auto g0 = grammar::Grammar(std::map<fsm::Type,std::vector<std::vector<fsm::Type>>>{{
    {"Start",{{"E"}}},
        {"E",{{"T"},{"E","+","T"}}},
        {"T",{{"F"},{"T","*","E"}}},
        {"F",{{"id"},{"(","E",")"}}},
}});

auto g1 = grammar::Grammar(std::map<fsm::Type,std::vector<std::vector<fsm::Type>>>{{
    {"Start",{{"S"}}},
        {"S",{{"A"},{"B"}}},
        {"A",{{"C"}}},
        {"B",{{"D"}}},
        {"C",{{"a"}}},
        {"D",{{"b"}}},
}});

TEST_CASE("item_less"){
    auto item1 = fsm::Item<&g0>("E",g0.der("E",0));
    auto item2 = fsm::Item<&g0>("E",g0.der("E",1));
    auto item3 = fsm::Item<&g0>("T",g0.der("T",0));
    auto item4 = item2.shift("E");
    REQUIRE(((item1 < item2) != (item2 < item1)));
    REQUIRE(((item1 < item3) != (item3 < item1)));
    REQUIRE(((item1 < item4) != (item4 < item1)));
}

TEST_CASE("computes_closure_of_single_element"){
    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start",0))});
    REQUIRE(init.size() == 7);
}
TEST_CASE("computes_closure_of_multiple_elements"){
    fsm::ItemSet<&g1> init = fsm::ItemSet<&g1>(std::set<fsm::Item<&g1>>{
        fsm::Item<&g1>("S",g1.der("S",0)),
        fsm::Item<&g1>("S",g1.der("S",1)),
    });
    REQUIRE(init.size() == 6);
}
TEST_CASE("computes_closure_of_multiple_elements_with_duplicates"){

    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{
        fsm::Item<&g0>("Start",g0.der("Start",0)),
        fsm::Item<&g0>("E",g0.der("E",0)),
        fsm::Item<&g0>("E",g0.der("E",1)),
        fsm::Item<&g0>("T",g0.der("T",0)),
        fsm::Item<&g0>("T",g0.der("T",1)),
        fsm::Item<&g0>("F",g0.der("F",0)),
        fsm::Item<&g0>("F",g0.der("F",1)),
        });
    REQUIRE(init.size() == 7);
}
TEST_CASE("itemset_equality"){
    fsm::ItemSet<&g0> init1 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start",0))});
    fsm::ItemSet<&g0> init2 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{
        fsm::Item<&g0>("Start",g0.der("Start",0)),
        fsm::Item<&g0>("E",g0.der("E",0)),
        fsm::Item<&g0>("E",g0.der("E",1)),
        fsm::Item<&g0>("T",g0.der("T",0)),
        fsm::Item<&g0>("T",g0.der("T",1)),
        fsm::Item<&g0>("F",g0.der("F",0)),
        fsm::Item<&g0>("F",g0.der("F",1)),
        });
    REQUIRE(init1 == init2);
}
