#ifndef _DECAF_LEXER_
#define _DECAF_LEXER_
#include <iostream>
#include "token.h"
namespace decaf_lexer{
class Lexer{
    public:
        Lexer(std::istream& stream) : input_stream(stream){};
        Lexer(Lexer& l) = delete; //Should not be copyable
        token::Token next_token();
        void print_tokens(){
            for(token::Token next = next_token(); next.type != token::TokenType::END; next = next_token()){
                std::cout<<next.to_string();
            }
            std::cout<<token::Token(token::TokenType::END, "").to_string()<<std::endl;
        }
    private:
        std::istream& input_stream;
        struct HelperMethods; 

};
} //namespace decaf_lexer
#endif // _DECAF_LEXER_
