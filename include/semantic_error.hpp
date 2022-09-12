//
// Created by user on 22.04.2017.
//

#ifndef FORTRAN_COMPILER_SEMANTIC_ERROR_HPP
#define FORTRAN_COMPILER_SEMANTIC_ERROR_HPP

#include "message.hpp"

class semantic_error : public message {
public:
    semantic_error(std::string msg);
    ~semantic_error();
};


#endif //FORTRAN_COMPILER_SEMANTIC_ERROR_HPP
