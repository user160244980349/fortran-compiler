//
// Created by user on 16.04.17.
//

#ifndef FORTRAN_COMPILER_VARIABLE_HPP
#define FORTRAN_COMPILER_VARIABLE_HPP

#include <string>
#include "token_type_enum.hpp"

class variable {
private:
    int id;
    static int counter;
public:
    std::string name;
    std::string asm_name;
    int value;
    token_type_enum type;
    bool initialized;
    variable();
    variable(std::string name, int value, bool initialized);
    variable(const variable &obj);
};

#endif //FORTRAN_COMPILER_VARIABLE_HPP
