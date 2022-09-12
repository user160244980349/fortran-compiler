//
// Created by user on 28.04.17.
//

#ifndef FORTRAN_COMPILER_CODEGEN_ERROR_H
#define FORTRAN_COMPILER_CODEGEN_ERROR_H

#include <string>
#include "message.hpp"

class codegen_error: public message {
public:
    codegen_error(std::string);
    ~codegen_error();
};


#endif //FORTRAN_COMPILER_CODEGEN_ERROR_H
