#include "fsm.h"
#include "grammar.h"
#include "catch2/catch.hpp"


TEST_CASE("computes_closure_properly"){
    std::map<fsm::Type,std::vector<std::vector<fsm::Type>>> derivations = {{
        {"Start",{{"E"}}},
        {"E",{{"T"},{"E","+","T"}}},
        {"T",{{"F"},{"T","*","E"}}},
        {"F",{{"id"},{"(","E",")"}}},
    }};
    auto g = grammar::Grammar(derivations);
    std::vector<fsm::Type> der = derivations.at("Start").at(0);
    fsm::ItemSet init = fsm::ItemSet(std::set<fsm::Item>{fsm::Item(&g,"Start",der)});
    REQUIRE(init.size() == 7);
}
