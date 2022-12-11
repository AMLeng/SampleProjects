//Lexical analyzer for the "Decaf" programming language
//See web.stanford.edu/class/archive/cs/cs143/cs143.1128
#include "decaf_lexer.h"
#include <cctype>
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>
namespace decaf_lexer{
using token::Token;
using token::TokenType;
struct Lexer::HelperMethods{
    static void ignore_space(Lexer& l){
        char next_char = l.input_stream.peek();
        while(std::isspace(next_char)){
            l.input_stream.ignore(1);
            next_char = l.input_stream.peek();
        }
    }
    static bool is_keyword(std::string word){
        return word == "void"
            || word == "int"
            || word == "double"
            || word == "bool"
            || word == "string"
            || word == "class"
            || word == "interface"
            || word == "null"
            || word == "this"
            || word == "extends"
            || word == "implements"
            || word == "for"
            || word == "while"
            || word == "if"
            || word == "else"
            || word == "return"
            || word == "break"
            || word == "new"
            || word == "NewArray"
            || word == "Print"
            || word == "ReadInteger"
            || word == "ReadLine";
    }
};

//Remains to implement: Int, Double
//Need tests for operator, string
Token Lexer::next_token(){
    Lexer::HelperMethods::ignore_space(*this);
    char c = input_stream.get();
    if(c== EOF){
        return Token(TokenType::END);
    }
    //Handle comments---note that we don't consume any more of the input if we're not starting a comment
    //Since '/' could also be a division operator
    if(c == '/'){
        if(input_stream.peek() == '/'){
            input_stream.get(); //Discard second '/'
            std::string comment;
            std::getline(input_stream, comment);
            return Token(TokenType::COMMENT);
        }
        if(input_stream.peek() == '*'){
            char n = input_stream.get();
            input_stream.get(n);
            while(n != '*' || input_stream.peek() != '/'){
                input_stream.get(n);
            }
            return Token(TokenType::COMMENT);
        }
    }
    //Handle keywords, bools, and identifiers
    if(std::isalpha(c)){
        std::string token_value = std::string(1,c);
        while(std::isalpha(input_stream.peek())){
            token_value.push_back(input_stream.get());
        }
        if(input_stream.peek() != '_'){
            if(token_value == "true" || token_value == "false"){
                return Token(TokenType::Bool, token_value);
            }
            if(Lexer::HelperMethods::is_keyword(token_value)){
                return Token(TokenType::Keyword, token_value);
            }
        }
        while(std::isalpha(input_stream.peek()) || input_stream.peek() == '_'){
            token_value.push_back(input_stream.get());
        }
        if(token_value.size() > 31){
            throw std::runtime_error("Invalid identifier \"" + token_value + "\" of length longer than 31 characters");
        }
        return Token(TokenType::Identifier, token_value);
    }
    //Handle all punctuation
    if(c == '('){
        return Token(TokenType::LParen, '(');
    }
    if(c == ')'){
        return Token(TokenType::RParen, ')');
    }
    if(c =='['){
        return Token(TokenType::LBrack, '[');
    }
    if(c ==']'){
        return Token(TokenType::RBrack, ']');
    }
    if(c=='{'){
        return Token(TokenType::LBrace, '{');
    }
    if(c =='}'){
        return Token(TokenType::RBrace, '}');
    }
    if(c ==';'){
        return Token(TokenType::Semicolon, ';');
    }
    if(c =='.'){
        return Token(TokenType::Period, '.');
    }
    if(c ==','){
        return Token(TokenType::Comma, ',');
    }
    //Handle strings
    if(c =='"'){
        std::string token_value = std::string(1,c);
        token_value.push_back(input_stream.get());
        while(input_stream.peek() != '"'){
            if(input_stream.peek() == '\n'){
                throw std::runtime_error("String beginning with \"" + token_value + "\" containes newline character");
            }
            if(input_stream.peek() == EOF){
                throw std::runtime_error("End of file reached while reading string beginning with \""+token_value+'"');
            }
            token_value.push_back(input_stream.get());
        }
        assert(input_stream.peek() == '"');
        token_value.push_back(input_stream.get());
        return Token(TokenType::String, token_value);
    }

    //Handle operators
    std::string first_two = std::string(1,c);
    //Unambiguous single char operators; note that we've already dealt with
    //The possibility that '/' starts a comment
    if(c == '+' || c == '-' || c == '*' || c == '/'){
        return Token(TokenType::Operator, c);
    }
    if(c == '&' || c == '|'){
        if(input_stream.get() == c){
            return Token(TokenType::Operator, std::string(c,2));
        }else{
            throw std::runtime_error("Invalid syntax while reading operator beginning with \'" + std::string(c,1) + "\'");
        }
    }
    if(c == '<' || c == '=' || c == '>' || c == '!'){
        std::string token_value = std::string(1,c);
        char d = input_stream.peek();
        if(d == '='){
            input_stream.get();
            token_value.push_back(d);
        }
        return Token(TokenType::Operator, token_value);
    }

    //Handle ints

    throw std::runtime_error("Unknown Symbol at character with int value " + static_cast<int>(c));
};




} //namespace decaf_lexer
