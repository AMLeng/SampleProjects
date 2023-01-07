#include "parser.cpp"
#include "decaf_grammar.h"
#include "catch2/catch.hpp"

//Parser takes 20-30 seconds to construct and has 217 states
//So we only construct it once

parser::Parser<&decaf_grammar::decaf_grammar> my_parser{};
TEST_CASE("make_parser"){
    std::cout<<my_parser.state_count()<<std::endl;
}
