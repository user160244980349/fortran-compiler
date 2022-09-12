cmake_minimum_required(VERSION 3.7)
project(fortran_compiler)

include_directories ("${PROJECT_SOURCE_DIR}/include")

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/message.hpp
        include/lexical_analyzer.hpp
        include/lexical_error.hpp
        include/token.hpp
        include/lexical_error.hpp
        include/syntax_error.hpp
        include/input_key.hpp
        include/item_key.hpp
        include/result_key.hpp
        include/token_type_enum.hpp
        include/statement_enum.hpp
        include/output_enum.hpp
        include/transition_table.hpp
        include/syntax_analyzer.hpp
        include/variable.hpp
        include/semantic_error.hpp
        include/code_generator.hpp
        include/codegen_error.hpp
        include/expression_tree.hpp
        include/tree_node.hpp
        message/message.cpp
        lexical-analyzer/token.cpp
        lexical-analyzer/lexical_analyzer.cpp
        lexical-analyzer/lexical_error.cpp
        syntax-analyzer/syntax_analyzer.cpp
        syntax-analyzer/syntax_error.cpp
        table/variable.cpp
        syntax-analyzer/semantic_error.cpp
        code-generator/code_generator.cpp
        code-generator/codegen_error.cpp
        expression-analyzer/tree_node.cpp
        expression-analyzer/expression_tree.cpp
        main.cpp
        table/function.cpp
        include/function.hpp table/table.cpp include/table.hpp table/function_call.cpp include/function_call.hpp)

add_executable(fortran_compiler ${SOURCE_FILES})