//
// Created by user on 07.04.17.
//

#include <string>
#include <iostream>
#include "message.hpp"

message::message(std::string msg) {
    std::cout << "Сообщение: " << msg << std::endl;
    delete this;
}

message::message() {

}

message::~message() {

}
