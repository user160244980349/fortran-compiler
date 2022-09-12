//
// Created by user on 07.04.17.
//

#include <string>
#include "token.hpp"

unsigned int token::priority() const {
    switch(type)
    {
        case PLUS:
        case MINUS:
            return 2;
        case MUL:
        case DIV:
            return 4;
        case LESS:
        case MORE:
        case NOTEQUALS:
        case ISEQUALS:
        case LESSOREQUALS:
        case MOREOREQUALS:
            return 6;
        case AND:
        case OR:
            return 8;
        default:
            return 100;
    }
}

bool token::operator<=(const token &obj) {
    return (priority() <= obj.priority());
}