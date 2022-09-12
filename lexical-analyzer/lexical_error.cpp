//
// Created by user on 07.04.17.
//

#include <string>
#include <iostream>
#include "lexical_error.hpp"

lexical_error::lexical_error(std::string msg) {
    std::cout << "Ошибка лексического анализатора: " << msg << std::endl;
    delete this;
}

lexical_error::~lexical_error() {

}