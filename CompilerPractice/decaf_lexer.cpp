//Lexical analyzer for the "Decaf" programming language
//See web.stanford.edu/class/archive/cs/cs143/cs143.1128
#include "decaf_lexer.h"
#include <cctype>
#include <vector>
#include <string>
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

//So far only keywords and identifiers implemented
Token Lexer::next_token(){
    if(input_stream.peek()== EOF){
        return Token(TokenType::END, "");
    }
    Lexer::HelperMethods::ignore_space(*this);
    if(std::isalpha(input_stream.peek())){
        std::string token_value = "";
        //Either a keyword or an identifier
        while(std::isalpha(input_stream.peek())){
            token_value.push_back(input_stream.get());
            if(token_value.size() > 31){
                std::cerr<<"Invalid identifier \""<<token_value<<"\"of length longer than 31 characters"<<std::endl;
                return Token(TokenType::ERR, "");
            }
        }
        if(Lexer::HelperMethods::is_keyword(token_value)){
            return Token(TokenType::Keyword, token_value);
        }
        while(std::isalpha(input_stream.peek()) || input_stream.peek() == '_'){
            token_value.push_back(input_stream.get());
            if(token_value.size() > 31){
                std::cerr<<"Invalid identifier \""<<token_value<<"\"of length longer than 31 characters"<<std::endl;
                return Token(TokenType::ERR, token_value);
            }
        }
        return Token(TokenType::Identifier, token_value);
    }
    char c = input_stream.get();
    std::cerr<<"Unknown Symbol at "<<c<<std::endl;
    return Token(TokenType::ERR, std::string(1,c));
};




} //namespace decaf_lexer
