#include "decaf_lexer.h"
#include <iostream>


int main(){
    decaf_lexer::Lexer lexer(std::cin); //No copy or move constructor so must call original constructor directly
    lexer.print_tokens();
}
