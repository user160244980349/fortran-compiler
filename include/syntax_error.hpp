//
// Created by user on 08.04.17.
//

#ifndef FORTRAN_COMPILER_SYNTAXERROR_HPP
#define FORTRAN_COMPILER_SYNTAXERROR_HPP

#include <string>
#include "message.hpp"

class syntax_error : public message {
public:
    syntax_error(std::string);
    ~syntax_error();
};


#endif //FORTRAN_COMPILER_SYNTAXERROR_HPP
