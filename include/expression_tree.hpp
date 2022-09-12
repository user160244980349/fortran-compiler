//
// Created by user on 28.04.17.
//

#ifndef FORTRAN_COMPILER_EXPRESSION_TREE_HPP
#define FORTRAN_COMPILER_EXPRESSION_TREE_HPP

#include "iostream"
#include <list>
#include "tree_node.hpp"

template<class T> // Шаблонный класс дерева
class expression_tree
{
public:
    tree_node<T>* root;
    ~expression_tree() {
        delete root;
    }
    std::list<T> reverse() {
        std::list<T> res;
        root->reverse(res);
        return res;
    }
};

#endif //FORTRAN_COMPILER_EXPRESSION_TREE_HPP
