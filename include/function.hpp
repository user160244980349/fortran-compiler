//
// Created by user on 09.05.17.
//

#ifndef FORTRAN_COMPILER_FUNCTION_HPP
#define FORTRAN_COMPILER_FUNCTION_HPP

#include <string>
#include <token_type_enum.hpp>

class function {
private:
    int id;
    static int counter;
public:
    std::string name;
    int arg_count;
    bool returning;
    function();
    function(std::string name, int value, bool initialized);
    function(const function &obj);
};


#endif //FORTRAN_COMPILER_FUNCTION_HPP
