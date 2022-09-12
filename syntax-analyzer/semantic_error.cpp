//
// Created by user on 22.04.2017.
//

#include <string>
#include <iostream>
#include "semantic_error.hpp"

semantic_error::semantic_error(std::string msg) {
    std::cout << "Ошибка семантического анализатора: " << msg << std::endl;
    delete this;
}

semantic_error::~semantic_error() {

}