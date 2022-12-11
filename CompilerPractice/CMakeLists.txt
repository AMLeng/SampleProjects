cmake_minimum_required(VERSION 3.18)

include_directories(include)
set (CMAKE_CXX_STANDARD_REQUIRED 17)
find_package(Catch2 2 REQUIRED)
#add_executable(lexer_tests lexer/decaf_lexer.cpp lexer/lexer_test.cpp)
#set_target_properties(lexer_tests PROPERTIES SUFFIX ".out")
#target_link_libraries(lexer_tests PRIVATE Catch2::Catch2WithMain)
project(DecafCompiler 
    LANGUAGES 
        CXX
)

add_executable(lex_and_print lexer/decaf_lexer.cpp lexer/lex_and_print.cpp)
set_target_properties(lex_and_print PROPERTIES SUFFIX ".out")
