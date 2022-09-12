//
// Created by user on 07.04.17.
//

#ifndef FORTRAN_COMPILER_LEXICALANALYZER_HPP
#define FORTRAN_COMPILER_LEXICALANALYZER_HPP

#include <fstream>
#include "token.hpp"
#include "token_type_enum.hpp"
#include "transition_table.hpp"

class lexical_analyzer {
private:
    struct {
        int pos;
        int line;
    } position_in_code;
    struct {
        std::ios::pos_type stream_pos;
        int pos;
        int line;
    } bookmark;
    const char* filename;
    std::fstream filestream;
    transition_table table;
    transition_table make_table();
    void backstep();
public:
    lexical_analyzer(char* filename);
    ~lexical_analyzer();
    bool read_token(token&);
    void save_statement();
    void roll_back();
    std::string get_pos();
};

#endif //FORTRAN_COMPILER_LEXICALANALYZER_HPP
