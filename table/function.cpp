//
// Created by user on 09.05.17.
//

#include "function.hpp"

int function::counter = 1;

function::function() {
    this->id = ++counter;
    this->arg_count = 0;
    this->returning = false;
}

function::function(const function &obj) {
    this->name = obj.name;
    this->arg_count = obj.arg_count;
    this->returning = obj.returning;
}