#ifndef _TOKEN_
#define _TOKEN_
#include <utility>
#include <string>
namespace token{
    //Tokens for the Lexer
enum class TokenType{Keyword, Identifier, Operator, Bool, Int, Double, String, 
            LParen, RParen, LBrack, RBrack, LBrace, RBrace, Semicolon, Period, Comma, COMMENT, END};
struct Token{
    Token(TokenType t, std::string v) : type(t), value(v){}
    Token(TokenType t, char c) : type(t), value(std::string(1,c)){}
    Token(TokenType t) : type(t), value(){}
    TokenType type;
    std::string value;
    std::string to_string(){
        std::string output;
        switch(type){
            case TokenType::Keyword:
                output = "Keyword ";
                break;
            case TokenType::Identifier:
                output = "Identifier ";
                break;
            case TokenType::Operator:
                output = "Operator ";
                break;
            case TokenType::Bool:
                output = "Bool ";
                break;
            case TokenType::Int:
                output = "Int ";
                break;
            case TokenType::Double:
                output = "Double ";
                break;
            case TokenType::String:
                output = "String ";
                break;
            case TokenType::END:
                output = "End of input reached\n";
                break;
            case TokenType::COMMENT:
                output = "Comment here\n";
                break;
            case TokenType::Semicolon:
                output = ";\n";
                break;
            case TokenType::LBrace:
                output = "{\n";
                break;
            case TokenType::RBrace:
                output = "}\n";
                break;
            default: 
                output = value;
                break;
        }
        return output;
    }
};
} //namespace token
#endif // _TOKEN_
