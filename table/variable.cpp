//
// Created by user on 16.04.17.
//

#include <list>
#include "variable.hpp"

int variable::counter = 1;

variable::variable() {
    this->id = ++counter;
}

variable::variable(const variable &obj) {
    this->id = obj.id;
    this->name = obj.name;
    this->type = obj.type;
    this->value = obj.value;
    this->asm_name = obj.asm_name;
    this->initialized = obj.initialized;
}