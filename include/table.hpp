//
// Created by user on 16.04.17.
//

#ifndef FORTRAN_COMPILER_IDENTIFIERTABLE_HPP
#define FORTRAN_COMPILER_IDENTIFIERTABLE_HPP

#include <vector>
#include <list>
#include <string>
#include "variable.hpp"

template <typename T>
class table
{
public:
    typedef typename std::vector<T>::iterator iterator;
    iterator begin(){
        return items.begin();
    }
    iterator end() {
        return items.end();
    }
    void push(T& var) {
        items.push_back(var);
    }
    bool get_by_name(const std::string &name, T &var) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].name == name) {
                var = items[i];
                return true;
            }
        }
        return false;
    }
    bool get_ptr_by_name(const std::string name, T* &var) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].name == name) {
                var = &(items[i]);
                return true;
            }
        }
        return false;
    }
    bool set_by_name(const std::string &name, T &var) {
        for(int i = 0; i < items.size(); i++) {
            if(items[i].name == name) {
                items[i] = var;
                return true;
            }
        }
        return false;
    }
private:
    std::vector<T> items;
};

#endif //FORTRAN_COMPILER_IDENTIFIERTABLE_HPP
