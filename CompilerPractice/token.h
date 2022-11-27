#ifndef _TOKEN_
#define _TOKEN_
#include <utility>
#include <string>
namespace token{
enum class TokenType{Keyword, Identifier, Operator, Bool, Int, Double, String, END, ERR};
struct Token{
    Token(TokenType t, std::string v) : type(t), value(v){}
    TokenType type;
    std::string value;
    std::string to_string(){
        std::string output;
        switch(type){
            case TokenType::Keyword:
                output = "Keyword: ";
                break;
            case TokenType::Identifier:
                output = "Identifier: ";
                break;
            case TokenType::Operator:
                output = "Operator: ";
                break;
            case TokenType::Bool:
                output = "Bool: ";
                break;
            case TokenType::Int:
                output = "Int: ";
                break;
            case TokenType::Double:
                output = "Double: ";
                break;
            case TokenType::String:
                output = "String: ";
                break;
            case TokenType::END:
                output = "End of input reached";
                break;
            case TokenType::ERR:
                output = "Error on: ";
                break;
        }
        return output + value;
    }
};
} //namespace token
#endif // _TOKEN_
