//
// Created by user on 08.04.17.
//

#ifndef FORTRAN_COMPILER_FSMTOKENENUM_HPP
#define FORTRAN_COMPILER_FSMTOKENENUM_HPP

enum token_type_enum {
    RESULT,
    FUNCTION,
    REAL,
    MOREOREQUALS,
    LESSOREQUALS,
    OR,
    AND,
    ISEQUALS,
    NOTEQUALS,
    CONTINUE,
    EXIT,
    PRINT,
    IDENT,
    DIGIT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUALS,
    LBR,
    RBR,
    TYPE,
    MORE,
    LESS,
    COMMA,
    NL,
    IF,
    THEN,
    ELSE,
    WHILE,
    DO,
    END,
    PROGRAM,
    INTEGER,
};

#endif //FORTRAN_COMPILER_FSMTOKENENUM_HPP
