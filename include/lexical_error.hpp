//
// Created by user on 07.04.17.
//

#ifndef FORTRAN_COMPILER_LEXICALERROR_HPP
#define FORTRAN_COMPILER_LEXICALERROR_HPP

#include <string>
#include "message.hpp"

class lexical_error : public message {
public:
    lexical_error(std::string);
    ~lexical_error();
};

#endif //FORTRAN_COMPILER_LEXICALERROR_HPP
