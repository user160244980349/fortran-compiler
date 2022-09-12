//
// Created by user on 23.04.2017.
//

#ifndef FORTRAN_COMPILER_CODE_GENERATOR_HPP
#define FORTRAN_COMPILER_CODE_GENERATOR_HPP

#include <string>

class code_generator {
public:
    void add_line(std::string&);
    void out(std::string&);
    void out(const char*);
    void declare_block();
private:
    std::string data;
    std::string code;
};

#endif //FORTRAN_COMPILER_CODE_GENERATOR_HPP
