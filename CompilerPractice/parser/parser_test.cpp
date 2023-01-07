#include "parser.cpp"
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

//Grammar 4.55 on page 263 of the dragon book
auto g5 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"Start",{{"S"}}},
    {"S",{{"C","C"}}},
    {"C",{{"d"},{"c","C"}}},
}});

//Lvalue Rvalue grammar
auto g6 = grammar::Grammar(std::map<parser::Type,std::vector<std::vector<parser::Type>>>{{
    {"Start",{{"S"}}},
    {"S",{{"L","=","R"},{"R"}}},
    {"L",{{"id"},{"*","R"}}},
    {"R",{{"L"}}},
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

TEST_CASE("computes_closure_of_single_element"){
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
    REQUIRE(((init1 < init2) != (init2 < init1)));
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
TEST_CASE("start_generates_inputend_lookahead"){
    auto i = parser::Item<&g2>("Start",g2.der("Start").at(0));
    parser::ItemSet<&g2> init = parser::ItemSet<&g2>(std::set<parser::Item<&g2>>{i});
    REQUIRE(init.lookaheads(i) == std::set<std::string>{"$"});
}
TEST_CASE("closures_propagates_inputend_lookahead"){
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    auto all_items = std::set<parser::Item<&g0>>{
        parser::Item<&g0>("Start",g0.der("Start").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(0)),
        parser::Item<&g0>("E",g0.der("E").at(1)),
        parser::Item<&g0>("T",g0.der("T").at(0)),
        parser::Item<&g0>("T",g0.der("T").at(1)),
        parser::Item<&g0>("F",g0.der("F").at(0)),
        parser::Item<&g0>("F",g0.der("F").at(1)),
        };
    for(auto i : all_items){
        REQUIRE(init.lookaheads(i).count("$") == 1);
    }
}

TEST_CASE("closures_generates_spontaneous_lookaheads"){
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    auto i = parser::Item<&g0>("E",{"T"});
    REQUIRE(init.lookaheads(i) == std::set<std::string>{"$","+"});
    i = parser::Item<&g0>("T",{"T","*","F"});
    REQUIRE(init.lookaheads(i) == std::set<std::string>{"$","+","*"});
    i = parser::Item<&g0>("F",{"id"});
    REQUIRE(init.lookaheads(i) == std::set<std::string>{"$","+","*"});
}

TEST_CASE("lr_zero_itemset_shift"){
    //See figure 4.31 on page 244 of the Dragon Book for the sizes of the sets
    //We test, in order, states 1,6,9,7,10
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    auto current = init.shift("E");
    REQUIRE(current.size() == 2);
    current = current.shift("+");
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
    //See figure 4.31 on page 244 of the Dragon Book for the complete LR(0) fsm
    parser::Parser<&g0> my_parser{};
    REQUIRE(my_parser.state_count() == 12);
}
TEST_CASE("parser_transitions"){
    //See figure 4.31 on page 244 of the Dragon Book for the complet lr(0) fsm
    parser::Parser<&g0> my_parser{};
    parser::ItemSet<&g0> init = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("Start",g0.der("Start").at(0))});
    parser::ItemSet<&g0> end = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{parser::Item<&g0>("F",{{"(","E",")"}}).shift().shift().shift()});
    auto current_pointer = my_parser.shift(my_parser.find(init),"(");
    current_pointer = my_parser.shift(current_pointer,"T");
    current_pointer = my_parser.shift(current_pointer,"*");
    current_pointer = my_parser.shift(current_pointer,"(");
    current_pointer = my_parser.shift(current_pointer,"E");
    current_pointer = my_parser.shift(current_pointer,")");
    REQUIRE(*current_pointer == end);
    REQUIRE(my_parser.shift(current_pointer,")") == nullptr);
}
TEST_CASE("parser_lookaheads"){
    //See figure 4.41 on page 262 of the Dragon Book for the LR(1) graph
    //Which gets collapsed into an LALR(1) graph that we use
    parser::Parser<&g5> my_parser{};
    REQUIRE(my_parser.state_count() == 7);
    auto item1 = parser::Item<&g5>("C",g5.der("C").at(0)).shift();
    auto set1 = parser::ItemSet<&g5>(std::set<parser::Item<&g5>>{item1});
    auto set1_in_parser = my_parser.find(set1);
    REQUIRE(set1_in_parser->lookaheads(item1) == std::set<std::string>{"c","d","$"});
    auto item2 = parser::Item<&g5>("C",g5.der("C").at(1)).shift();
    auto set2 = parser::ItemSet<&g5>(std::set<parser::Item<&g5>>{item2});
    auto set2_in_parser = my_parser.find(set2);
    REQUIRE(set2_in_parser->lookaheads(item2) == std::set<std::string>{"c","d","$"});
}
TEST_CASE("parser_lookaheads_part_2"){
    //See figure 4.47 on page 275 of the Dragon Book for the lookahead table
    parser::Parser<&g6> my_parser{};
    REQUIRE(my_parser.state_count() == 10);
    auto item1 = parser::Item<&g6>("R",{"L"}).shift();
    auto set1 = parser::ItemSet<&g6>(std::set<parser::Item<&g6>>{item1});
    auto set1_in_parser = my_parser.find(set1);
    REQUIRE(set1_in_parser->lookaheads(item1) == std::set<std::string>{"=","$"});
    auto item2 = parser::Item<&g6>("S",{"L","=","R"}).shift().shift().shift();
    auto set2 = parser::ItemSet<&g6>(std::set<parser::Item<&g6>>{item2});
    auto set2_in_parser = my_parser.find(set2);
    REQUIRE(set2_in_parser->lookaheads(item2) == std::set<std::string>{"$"});
}
TEST_CASE("parser_lookaheads_part_3"){
    //See figure 4.47 on page 275 of the Dragon Book for the lookahead table
    parser::Parser<&g0> my_parser{};
    auto item1 = parser::Item<&g0>("F",{"(","E",")"}).shift();
    auto set1 = parser::ItemSet<&g0>(std::set<parser::Item<&g0>>{item1});
    auto set1_in_parser = my_parser.find(set1);
    REQUIRE(set1_in_parser->lookaheads(item1) == std::set<std::string>{"+","*","$",")"});
    auto item2 = parser::Item<&g0>("T",{"F"});
    REQUIRE(set1_in_parser->lookaheads(item2) == std::set<std::string>{")","+","*"});
    auto set2_in_parser = my_parser.shift(set1_in_parser, "F");
    auto item3 = parser::Item<&g0>("T",{"F"}).shift();
    REQUIRE(set2_in_parser->lookaheads(item3) == std::set<std::string>{")","+","*","$"});
}
TEST_CASE("parses_expression_grammar"){
    parser::Parser<&g0> my_parser{};
    std::vector<parser::Type> input = {{"(","id","*","id",")","*","id","+","id","*","id"}};
    REQUIRE(my_parser.parses(input.begin(),input.end()));
    //Extra paren
    input = std::vector<parser::Type>{{"(","id","*","id",")","*","id","+","id","*","id",")"}};
    REQUIRE(!my_parser.parses(input.begin(),input.end()));
    //Missing id
    input = std::vector<parser::Type>{{"(","id","*","id",")","*","id","+","id","*"}};
    REQUIRE(!my_parser.parses(input.begin(),input.end()));
}
