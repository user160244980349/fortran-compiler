//
// Created by user on 28.04.17.
//

#include <iostream>
#include "codegen_error.hpp"

codegen_error::codegen_error(std::string msg) {
    std::cout << "Ошибка генератора кода: " + msg;
}

codegen_error::~codegen_error() {

}