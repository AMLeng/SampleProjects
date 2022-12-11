#include "catch2/catch.hpp"
#include "decaf_lexer.h"
#include "token.h"
#include <sstream>
#include <stdexcept>
#include <cassert>

TEST_CASE("recognizes_empty_input_stream"){
    auto ss = std::stringstream("");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_identifier_not_keyword"){
    auto ss = std::stringstream("this_is_a_identifier");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Identifier);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_identifier_with_digits"){
    auto ss = std::stringstream("identifier_0");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Identifier);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("error_identifier_too_long"){
    auto ss = std::stringstream("this_identifier_is_longer_than_thirty_one_characters_and_thus_should_throw_an_error");
    decaf_lexer::Lexer l(ss);
    REQUIRE_THROWS_AS(l.next_token(),std::runtime_error);
}
TEST_CASE("recognizes_bool"){
    auto ss = std::stringstream("true false");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Bool);
    REQUIRE(l.next_token().type == token::TokenType::Bool);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_line_comment"){
    auto ss = std::stringstream("//This is a comment");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::COMMENT);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_multiline_comment"){
    auto ss = std::stringstream("/*This is a comment \n"
        "which takes up multiple lines \n"
        "and includes other syntax \n"
        "like the following chars: /*^%$#@!'\". \n"
        "But does not end until now.*/");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::COMMENT);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_punctuation"){
    auto ss = std::stringstream("()[]{};.,");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::LParen);
    REQUIRE(l.next_token().type == token::TokenType::RParen);
    REQUIRE(l.next_token().type == token::TokenType::LBrack);
    REQUIRE(l.next_token().type == token::TokenType::RBrack);
    REQUIRE(l.next_token().type == token::TokenType::LBrace);
    REQUIRE(l.next_token().type == token::TokenType::RBrace);
    REQUIRE(l.next_token().type == token::TokenType::Semicolon);
    REQUIRE(l.next_token().type == token::TokenType::Period);
    REQUIRE(l.next_token().type == token::TokenType::Comma);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_decimal_integers"){
    auto ss = std::stringstream("12 0012");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_hex_integers"){
    auto ss = std::stringstream("0x0 0X12aE 0X1EF");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_doubles"){
    auto ss = std::stringstream("12.13 1. 0.3 00.03");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_doubles_in_scientific_notation"){
    auto ss = std::stringstream("12.2E+2 10.e-2 0.3e001 003.3E3");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::Double);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_keywords"){
    auto ss = std::stringstream("void int double bool string class interface null this extends implements for while if else return break new NewArray Print ReadInteger ReadLine");
    decaf_lexer::Lexer l(ss);
    for(int i=0; i<22; i++){
        REQUIRE(l.next_token().type == token::TokenType::Keyword);
    }
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_strings"){
    auto ss = std::stringstream("\"This is a valid string\"");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::String);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_strings_with_single_quotes"){
    auto ss = std::stringstream("\"This is 'a' valid string\"");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::String);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("error_string_with_newline"){
    auto ss = std::stringstream("\"This is not \na valid string\"");
    decaf_lexer::Lexer l(ss);
    REQUIRE_THROWS_AS(l.next_token(),std::runtime_error);
}
TEST_CASE("error_incomplete_string"){
    auto ss = std::stringstream("\"This is not a valid string");
    decaf_lexer::Lexer l(ss);
    REQUIRE_THROWS_AS(l.next_token(),std::runtime_error);
}
TEST_CASE("recognizes_arithmetic_operators"){
    auto ss = std::stringstream("1+2\n"
        "2 * 3\n"
        "3 /2\n"
        "4- 2\n"
        "5% 2\n"
        "-3"
    );
    decaf_lexer::Lexer l(ss);
    for(int i=0; i<5; i++){
        REQUIRE(l.next_token().type == token::TokenType::Int);
        REQUIRE(l.next_token().type == token::TokenType::Operator);
        REQUIRE(l.next_token().type == token::TokenType::Int);
    }
    REQUIRE(l.next_token().type == token::TokenType::Operator);
    REQUIRE(l.next_token().type == token::TokenType::Int);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_logical_operators"){
    auto ss = std::stringstream("true && true\n"
        " true || false\n"
        "!true"
    );
    decaf_lexer::Lexer l(ss);
    for(int i=0; i<2; i++){
        REQUIRE(l.next_token().type == token::TokenType::Bool);
        REQUIRE(l.next_token().type == token::TokenType::Operator);
        REQUIRE(l.next_token().type == token::TokenType::Bool);
    }
    REQUIRE(l.next_token().type == token::TokenType::Operator);
    REQUIRE(l.next_token().type == token::TokenType::Bool);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_comparison_operators"){
    auto ss = std::stringstream("1<3 \n"
        " 2 >6\n"
        "3 == 6 \n"
        "4 <= 6 \n"
        "5 >= 6 \n"
        "6 != 6 \n"
    );
    decaf_lexer::Lexer l(ss);
    for(int i=0; i<6; i++){
        REQUIRE(l.next_token().type == token::TokenType::Int);
        REQUIRE(l.next_token().type == token::TokenType::Operator);
        REQUIRE(l.next_token().type == token::TokenType::Int);
    }
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("recognizes_assignment_operator"){
    auto ss = std::stringstream("test_obj = true");
    decaf_lexer::Lexer l(ss);
    REQUIRE(l.next_token().type == token::TokenType::Identifier);
    REQUIRE(l.next_token().type == token::TokenType::Operator);
    REQUIRE(l.next_token().type == token::TokenType::Bool);
    REQUIRE(l.next_token().type == token::TokenType::END);
}
TEST_CASE("error_unpaired_and_or"){
    auto ss = std::stringstream("true&false false|true");
    decaf_lexer::Lexer l(ss);
    for(int i=0; i<2; i++){
        REQUIRE(l.next_token().type == token::TokenType::Bool);
        REQUIRE_THROWS_AS(l.next_token(),std::runtime_error);
        REQUIRE(l.next_token().type == token::TokenType::Bool);
    }
    REQUIRE(l.next_token().type == token::TokenType::END);
}
