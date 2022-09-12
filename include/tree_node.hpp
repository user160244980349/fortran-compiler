//
// Created by user on 28.04.17.
//

#ifndef FORTRAN_COMPILER_TREE_NODE_HPP
#define FORTRAN_COMPILER_TREE_NODE_HPP

#include <list>

template<class T>
class tree_node {
public:
    typedef tree_node<T>* tree_node_ptr;
    T value;
    std::list<tree_node_ptr> children;
    ~tree_node() {
        typename std::list<tree_node_ptr>::iterator it;
        for(it = children.begin(); it != children.end(); it++) {
            if(*it != nullptr) {
                delete *it;
            }
        }
    }
    void reverse(std::list<T> &res) {
        typename std::list<tree_node_ptr>::iterator it;
        for(it = children.begin(); it != children.end(); it++) {
            if(*it != nullptr) {
                (*it)->reverse(res);
            }
        }
        res.push_back(value);
    }
};

#endif //FORTRAN_COMPILER_TREE_NODE_HPP
