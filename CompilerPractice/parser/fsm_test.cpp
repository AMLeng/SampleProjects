#include "fsm.h"
#include "grammar.h"
#include "catch2/catch.hpp"

//Expression Grammar; is not LR(0) but is SLR(1)
auto g0 = grammar::Grammar(std::map<fsm::Type,std::vector<std::vector<fsm::Type>>>{{
    {"Start",{{"E"}}},
        {"E",{{"T"},{"E","+","T"}}},
        {"T",{{"F"},{"T","*","F"}}},
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
    auto item1 = fsm::Item<&g0>("E",g0.der("E").at(0));
    auto item2 = fsm::Item<&g0>("E",g0.der("E").at(1));
    auto item3 = fsm::Item<&g0>("T",g0.der("T").at(0));
    auto item4 = item2.shift();
    REQUIRE(((item1 < item2) != (item2 < item1)));
    REQUIRE(((item1 < item3) != (item3 < item1)));
    REQUIRE(((item1 < item4) != (item4 < item1)));
}

TEST_CASE("computes_closure_of_single_element"){
    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start").at(0))});
    REQUIRE(init.size() == 7);
}
TEST_CASE("computes_closure_of_multiple_elements"){
    fsm::ItemSet<&g1> init = fsm::ItemSet<&g1>(std::set<fsm::Item<&g1>>{
        fsm::Item<&g1>("S",g1.der("S").at(0)),
        fsm::Item<&g1>("S",g1.der("S").at(1)),
    });
    REQUIRE(init.size() == 6);
}
TEST_CASE("computes_closure_of_multiple_elements_with_duplicates"){

    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{
        fsm::Item<&g0>("Start",g0.der("Start").at(0)),
        fsm::Item<&g0>("E",g0.der("E").at(0)),
        fsm::Item<&g0>("E",g0.der("E").at(1)),
        fsm::Item<&g0>("T",g0.der("T").at(0)),
        fsm::Item<&g0>("T",g0.der("T").at(1)),
        fsm::Item<&g0>("F",g0.der("F").at(0)),
        fsm::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(init.size() == 7);
}
TEST_CASE("itemset_equality"){
    fsm::ItemSet<&g0> init1 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start").at(0))});
    fsm::ItemSet<&g0> init2 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{
        fsm::Item<&g0>("Start",g0.der("Start").at(0)),
        fsm::Item<&g0>("E",g0.der("E").at(0)),
        fsm::Item<&g0>("E",g0.der("E").at(1)),
        fsm::Item<&g0>("T",g0.der("T").at(0)),
        fsm::Item<&g0>("T",g0.der("T").at(1)),
        fsm::Item<&g0>("F",g0.der("F").at(0)),
        fsm::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(init1 == init2);
}
TEST_CASE("itemset_less"){
    fsm::ItemSet<&g0> init1 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start").at(0))});
    fsm::ItemSet<&g0> init2 = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{
        fsm::Item<&g0>("E",g0.der("E").at(0)),
        fsm::Item<&g0>("E",g0.der("E").at(1)),
        fsm::Item<&g0>("T",g0.der("T").at(0)),
        fsm::Item<&g0>("T",g0.der("T").at(1)),
        fsm::Item<&g0>("F",g0.der("F").at(0)),
        fsm::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(((init1 < init2) || (init2 < init1)));
}
TEST_CASE("itemset_shift"){
    //See figure 4.31 on page 244 of the Dragon Book for the sizes of the sets
    //We test, in order, states 1,6,9,7,10
    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start").at(0))});
    auto current = init.shift("E");
    std::cout<<std::endl;
    REQUIRE(current.size() == 2);
    current = current.shift("+");
    std::cout<<std::endl;
    REQUIRE(current.size() == 5);
    current = current.shift("T");
    REQUIRE(current.size() == 2);
    current = current.shift("*");
    REQUIRE(current.size() == 3);
    {
    auto dead_end = current.shift("F");
    auto dead_end_test = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("T",g0.der("T").at(1)).shift().shift().shift()});
    REQUIRE(dead_end == dead_end_test);
    }
}

TEST_CASE("fsm_generation_correct_size"){
    //See figure 4.31 on page 244 of the Dragon Book for the complete FSM
    fsm::FSM<&g0> my_fsm{};
    REQUIRE(my_fsm.state_count() == 12);
}
TEST_CASE("fsm_transitions"){
    //See figure 4.31 on page 244 of the Dragon Book for the complete FSM
    fsm::FSM<&g0> my_fsm{};
    fsm::ItemSet<&g0> init = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("Start",g0.der("Start").at(0))});
    fsm::ItemSet<&g0> end = fsm::ItemSet<&g0>(std::set<fsm::Item<&g0>>{fsm::Item<&g0>("F",{{"(","E",")"}}).shift().shift().shift()});
    std::vector<fsm::Type> symbols = {{"(","T","*","(","E",")"}};
    auto current_pointer = my_fsm.eval(init,symbols.begin(),symbols.end());
    REQUIRE(*current_pointer == end);
    std::vector<fsm::Type> symbols2 = {{")"}};
    REQUIRE(my_fsm.eval(current_pointer,symbols2.begin(),symbols2.end()) == nullptr);
}
