#include "parser.h"
#include "grammar.h"
#include "catch2/catch.hpp"
#include <vector>
#include <set>
#include <map>

//Expression Grammar; is not LR(0) but is SLR(1)
auto g0 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"Start",{{"E"}}},
        {"E",{{"T"},{"E","+","T"}}},
        {"T",{{"F"},{"T","*","F"}}},
        {"F",{{"id"},{"(","E",")"}}},
}});

auto g1 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"Start",{{"S"}}},
        {"S",{{"A"},{"B"}}},
        {"A",{{"C"}}},
        {"B",{{"D"}}},
        {"C",{{"a"}}},
        {"D",{{"b"}}},
}});

auto g2 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"Start",{{"S"}}},
        {"S",{{"A","b","B","a"},{"B","a","A","b"}}},
        {"A",{{}}},
        {"B",{{"b"}}},
}});
//Actually part of the decaf grammar, with multiple empty productions
auto g3 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"StmtBlock",{{"VariableDecls", "Stmts"}}},
    {"VariableDecls",{{},{"VariableDecl","VariableDecls"}}},
    {"Stmts",{{},{"Stmt","Stmts"}}},
}});
//Highly recursive
auto g4 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"A",{{},{"B", "C"}}},
    {"B",{{},{"A","a"}}},
    {"C",{{"b"},{"B","c"}}},
}});

//Test grammar
TEST_CASE("terminals_and_nonterminals"){
    auto g0_terminals = std::set<parser::Type>{"(",")","+","*","id"};
    for(auto type : g0.types){
        REQUIRE((g0.is_terminal(type)== (g0_terminals.find(type) != g0_terminals.end())));
    }
    auto g1_terminals = std::set<parser::Type>{"a","b"};
    for(auto type : g1.types){
        REQUIRE((g1.is_terminal(type)== (g1_terminals.find(type) != g1_terminals.end())));
    }
    auto g2_terminals = std::set<parser::Type>{"a","b"};
    for(auto type : g2.types){
        REQUIRE((g2.is_terminal(type)== (g2_terminals.find(type) != g2_terminals.end())));
    }
}
TEST_CASE("produces_epsilon"){
    for(auto type : g0.types){
        REQUIRE(!g0.produces_epsilon(type));
    }
    for(auto type : g2.types){
        if(type == "A"){
            REQUIRE(g2.produces_epsilon(type));
        }else{
            REQUIRE(!g2.produces_epsilon(type));
        }
    }
    REQUIRE(g3.produces_epsilon("VariableDecls"));
    REQUIRE(g3.produces_epsilon("Stmts"));
    REQUIRE(g3.produces_epsilon("StmtBlock"));
    REQUIRE(g4.produces_epsilon("A"));
    REQUIRE(g4.produces_epsilon("B"));
    REQUIRE(!g4.produces_epsilon("C"));
}
TEST_CASE("first_sets_consist_of_terminals"){
    for(auto type : g0.types){
        for(auto t : g0.first_set(type)){
            REQUIRE(g0.is_terminal(t));
        }
    }
    for(auto type : g3.types){
        for(auto t : g3.first_set(type)){
            REQUIRE(g3.is_terminal(t));
        }
    }
    for(auto type : g4.types){
        for(auto t : g4.first_set(type)){
            REQUIRE(g4.is_terminal(t));
        }
    }
}
TEST_CASE("first_sets"){
    REQUIRE(g2.first_set("S") == std::set<std::string>{"b"});
    REQUIRE(g2.first_set("A").size() == 0);
    auto correct_set = std::set<std::string>{"a","b","c"};
    REQUIRE(g4.first_set("A") == correct_set);
    REQUIRE(g4.first_set("B") == correct_set);
    REQUIRE(g4.first_set("C") == correct_set);
}

//Test item class
TEST_CASE("item_less"){
    auto item1 = parser::Item<&g0>("E",g0.der("E").at(0));
    auto item2 = parser::Item<&g0>("E",g0.der("E").at(1));
    auto item3 = parser::Item<&g0>("T",g0.der("T").at(0));
    auto item4 = item2.shift();
    REQUIRE(((item1 < item2) != (item2 < item1)));
    REQUIRE(((item1 < item3) != (item3 < item1)));
    REQUIRE(((item1 < item4) != (item4 < item1)));
}
TEST_CASE("item_operations"){
    auto item = parser::Item<&g0>("F",g0.der("F").at(1));
    REQUIRE(item.next() == "(");
    item = item.shift();
    REQUIRE(item.next() == "E");
    item = item.shift();
    REQUIRE(item.next() == ")");
    item = item.shift();
    REQUIRE(item.at_end());
}


//TEST_CASE("first_set"){

/*TEST_CASE("computes_closure_of_single_element"){
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    REQUIRE(init.size() == 7);
}
TEST_CASE("computes_closure_of_multiple_elements"){
    parser::ItemSet<&g1> init = parser::ItemSet<&g1>(std::set<parser::Item<&g1>>{
        parser::Item<&g1>("S",g1.der("S").at(0)),
        parser::Item<&g1>("S",g1.der("S").at(1)),
    });
    REQUIRE(init.size() == 6);
}
TEST_CASE("computes_closure_of_multiple_elements_with_duplicates"){

    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{
        parser::Item<&g0>("Start",g0.der("Start").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(1)),
        parser::Item<&g0>("T",g0.der("T").at(0)),
        parser::Item<&g0>("T",g0.der("T").at(1)),
        parser::Item<&g0>("F",g0.der("F").at(0)),
        parser::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(init.size() == 7);
}
TEST_CASE("itemset_equality"){
    parser::ItemSet<&g0> init1 = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    parser::ItemSet<&g0> init2 = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{
        parser::Item<&g0>("Start",g0.der("Start").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(1)),
        parser::Item<&g0>("T",g0.der("T").at(0)),
        parser::Item<&g0>("T",g0.der("T").at(1)),
        parser::Item<&g0>("F",g0.der("F").at(0)),
        parser::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(init1 == init2);
}
TEST_CASE("itemset_less"){
    parser::ItemSet<&g0> init1 = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    parser::ItemSet<&g0> init2 = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{
        parser::Item<&g0>("E",g0.der("E").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(1)),
        parser::Item<&g0>("T",g0.der("T").at(0)),
        parser::Item<&g0>("T",g0.der("T").at(1)),
        parser::Item<&g0>("F",g0.der("F").at(0)),
        parser::Item<&g0>("F",g0.der("F").at(1)),
        });
    REQUIRE(((init1 < init2) || (init2 < init1)));
}
TEST_CASE("itemset_shift"){
    //See figure 4.31 on page 244 of the Dragon Book for the sizes of the sets
    //We test, in order, states 1,6,9,7,10
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
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
    auto dead_end_test = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("T",g0.der("T").at(1)).shift().shift().shift()});
    REQUIRE(dead_end == dead_end_test);
    }
}
TEST_CASE("parser_generation_correct_size"){
    //See figure 4.31 on page 244 of the Dragon Book for the complete Parser
    parser::Parser<&g0> my_parser{};
    REQUIRE(my_parser.state_count() == 12);
}
TEST_CASE("parser_transitions"){
    //See figure 4.31 on page 244 of the Dragon Book for the complete Parser
    parser::Parser<&g0> my_parser{};
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    parser::ItemSet<&g0> end = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("F",{{"(","E",")"}}).shift().shift().shift()});
    std::vector<parser::Type> symbols = {{"(","T","*","(","E",")"}};
    auto current_pointer = my_parser.eval(init,symbols.begin(),symbols.end());
    REQUIRE(*current_pointer == end);
    std::vector<parser::Type> symbols2 = {{")"}};
    REQUIRE(my_parser.eval(current_pointer,symbols2.begin(),symbols2.end()) == nullptr);
}*/
