//
// Created by user on 08.04.17.
//

#ifndef FORTRAN_COMPILER_syntax_analyzer_HPP
#define FORTRAN_COMPILER_syntax_analyzer_HPP

#include <fstream>
#include <list>
#include "token.hpp"
#include "code_generator.hpp"
#include "lexical_analyzer.hpp"
#include "tree_node.hpp"
#include "variable.hpp"
#include "function.hpp"
#include "table.hpp"
#include "function_call.hpp"

class syntax_analyzer {
private:
    lexical_analyzer lexical;
    table<variable> *idents_table;
    table<function> funcs_table;
    table<function_call> funcs_calls_table;
    code_generator code_gen;
    int calls_counter;
    int if_counter;
    int do_counter;
    int cmp_counter;
    int opened_do;
    int rbp_vars_offset;
    int rbp_params_offset;
    const char* outfile;
    bool start_lexma();
    bool definition();
    bool can_read(int n);
    bool if_statement();
    bool dowhile_statement();
    bool print();
    bool on_expression(std::list<token>&);
    bool on_terminal(std::list<token>&);
    bool lexma_is(token_type_enum, std::list<token>&);
    bool lexma_is(token_type_enum, token&);
    bool output(std::list<token>&);
    bool program_statement(token&);
    bool initialization();
    bool expression_to_tree(std::list<token> &_list, tree_node<token> &_root);
    int calculate_expression(std::list<token> &poliz);
    bool function_declare(token t);
    bool program_declare(token t);
    bool function_statement(token &t, token result);
    bool call_function(std::list<token> params, function func);
    bool code_to_poliz(std::list<token> &poliz);
    bool list_to_poliz(std::list<token> &poliz);
public:
    syntax_analyzer(char* ,const char*);
    ~syntax_analyzer();
    bool parse();
};

#endif //FORTRAN_COMPILER_syntax_analyzer_HPP
