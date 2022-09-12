//
// Created by user on 08.04.17.
//

#include <iostream>
#include "syntax_error.hpp"

syntax_error::syntax_error(std::string msg) {
    std::cout << "Ошибка синтаксического анализатора: " << msg << std::endl;
    delete this;
}

syntax_error::~syntax_error() {

}