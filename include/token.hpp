//
// Created by user on 07.04.17.
//

#ifndef FORTRAN_COMPILER_TOKEN_HPP
#define FORTRAN_COMPILER_TOKEN_HPP

#include <string>
#include "token_type_enum.hpp"

class token {
private:
public:
    std::string value;
    token_type_enum type;
    unsigned int priority() const;
    bool operator<=(const token &obj);
};

#endif //FORTRAN_COMPILER_TOKEN_HPP
