//
// Created by user on 23.04.2017.
//

#include <fstream>
#include "codegen_error.hpp"
#include "code_generator.hpp"

void code_generator::declare_block() {
    data += "\n.data\n";
    data += "\tformat:\t.asciz\t\"%d\\n\"\n";
}

void code_generator::add_line(std::string &str) {
    code += str;
    code += "\n";
}

void code_generator::out(std::string &str) {
    str = ".text\n";
    str += "\n.global main\n\n";
    str += code;
    str += "main:\n";
    str += "\tcall\tprogram\n";
    str += "\tmovq\t$1, %rax\n";
    str += "\tint\t\t$0x80\n";
    str += data;
}

void code_generator::out(const char* filename) {
    std::ofstream filestream;
    std::string a;
    filestream.open(filename);
    if(!filestream)
        throw new codegen_error("ошибка записи");
    out(a);
    filestream << a;
    filestream.close();
}
