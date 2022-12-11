cmake_minimum_required(VERSION 3.18)

set (CMAKE_CXX_STANDARD_REQUIRED 17)
find_package(Catch2 2 REQUIRED)
add_executable(lexer_tests decaf_lexer.cpp lexer_test.cpp)
set_target_properties(lexer_tests PROPERTIES SUFFIX ".out")
target_link_libraries(lexer_tests PRIVATE Catch2::Catch2WithMain)
project(DecafCompiler 
    LANGUAGES 
        CXX
)

#add_executable(decaf_compiler decaf_lexer.cpp lexer_test.cpp)
