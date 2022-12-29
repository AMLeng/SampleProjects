cmake_minimum_required(VERSION 3.18)

include_directories(include)
set (CMAKE_CXX_STANDARD_REQUIRED 17)
find_package(Catch2 2 REQUIRED)
project(DecafCompiler 
    LANGUAGES 
        CXX
)


#[[
add_executable(lexer_tests lexer/decaf_lexer.cpp lexer/lexer_test.cpp)
set_target_properties(lexer_tests PROPERTIES SUFFIX ".out")
target_link_libraries(lexer_tests PRIVATE Catch2::Catch2WithMain)
]]
add_executable(fsm_tests parser/fsm_test.cpp)
set_target_properties(fsm_tests PROPERTIES SUFFIX ".out")
target_link_libraries(fsm_tests PRIVATE Catch2::Catch2WithMain)

#[[
add_executable(print_lex lexer/decaf_lexer.cpp lexer/print_lexer_results.cpp)
set_target_properties(print_lex PROPERTIES SUFFIX ".out")
]]
