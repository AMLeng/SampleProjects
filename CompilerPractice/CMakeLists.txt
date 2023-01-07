cmake_minimum_required(VERSION 3.18)

include_directories(include)
set (CMAKE_CXX_STANDARD_REQUIRED 17)
find_package(Catch2 2 REQUIRED)
project(DecafCompiler 
    LANGUAGES 
        CXX
)


#[[ #Uncomment this block to generate tests
add_executable(lexer_tests lexer/decaf_lexer.cpp lexer/lexer_test.cpp)
set_target_properties(lexer_tests PROPERTIES SUFFIX ".out")
target_link_libraries(lexer_tests PRIVATE Catch2::Catch2WithMain)
]]
add_executable(parser_tests parser/parser_test.cpp)
set_target_properties(parser_tests PROPERTIES SUFFIX ".out")
target_link_libraries(parser_tests PRIVATE Catch2::Catch2WithMain)

add_executable(decaf_parser_tests parser/decaf_parser_test.cpp)
set_target_properties(decaf_parser_tests PROPERTIES SUFFIX ".out")
target_link_libraries(decaf_parser_tests PRIVATE Catch2::Catch2WithMain)

#[[ #Non-test intermediate debugging programs
add_executable(print_lex lexer/decaf_lexer.cpp lexer/print_lexer_results.cpp)
set_target_properties(print_lex PROPERTIES SUFFIX ".out")
]]
