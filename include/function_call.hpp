//
// Created by user on 09.05.17.
//

#ifndef FORTRAN_COMPILER_FUNCTION_CALL_HPP
#define FORTRAN_COMPILER_FUNCTION_CALL_HPP

#include <string>
#include <list>
#include <functional>
#include "token.hpp"
#include "function.hpp"

class function_call {
public:
    std::string name;
    function *parent;
    std::list<token> params;
    function_call() {};
    function_call(std::string name, std::list<token> params, function *parent) {
        this->name = name;
        this->params = params;
        this->parent = parent;
    }
    function_call(const function_call &obj) {
        this->name = obj.name;
        this->params = obj.params;
        this->parent = obj.parent;
    }
};


#endif //FORTRAN_COMPILER_FUNCTION_CALL_HPP
