//
// Created by user on 08.04.17.
//

#include <iostream>
#include <stack>
#include "syntax_error.hpp"
#include "semantic_error.hpp"
#include "tree_node.hpp"
#include "syntax_analyzer.hpp"
#include "expression_tree.hpp"

syntax_analyzer::syntax_analyzer(char* infile,const char* outfile) : lexical(infile), idents_table() {
    this->outfile = outfile;
    if_counter = 0;
    do_counter = 0;
    cmp_counter = 0;
    opened_do = 0;
    calls_counter = 0;
}

bool syntax_analyzer::parse () {
    token t;
    while (can_read(1)) {
        if (lexma_is(PROGRAM, t)) {
            program_declare(t);
            continue;
        }
        if (lexma_is(FUNCTION, t)) {
            function_declare(t);
            continue;
        }
    }
    return false;
}

bool syntax_analyzer::program_statement(token &t) {
    std::string out;
    while(can_read(1)) {
        if (lexma_is(END, t)) {
            if (lexma_is(PROGRAM, t)) {
                if (lexma_is(IDENT, t)) {
                    out = "\tmovq\t%rbp, %rsp";
                    code_gen.add_line(out);
                    out = "\tpopq\t%rbp";
                    code_gen.add_line(out);
                    out = "\tret\n";
                    code_gen.add_line(out);
                    code_gen.declare_block();
                    code_gen.out(outfile);
                    return true;
                } else
                    throw new syntax_error("пропущено название " + lexical.get_pos());
            } else
                throw new syntax_error("программа не закрыта " + lexical.get_pos());
        }
        if (!start_lexma())
            throw new syntax_error("неизвестная стартовая лексема " + t.value + " " + lexical.get_pos());
    }
    throw new syntax_error("программа не закрыта " + lexical.get_pos());
}

bool syntax_analyzer::function_statement(token &t, token result) {
    std::string out;
    variable var;
    while(can_read(1)) {
        if (lexma_is(END, t)) {
            if (lexma_is(FUNCTION, t)) {
                if (lexma_is(IDENT, t)) {
                    if (idents_table->get_by_name(result.value, var)) {
                        out = "\tmovq\t" + var.asm_name + ", %rax";
                        code_gen.add_line(out);
                    }
                    out = "\tmovq\t%rbp, %rsp";
                    code_gen.add_line(out);
                    out = "\tpopq\t%rbp";
                    code_gen.add_line(out);
                    out = "\tret\n";
                    code_gen.add_line(out);
                    return true;
                } else
                    throw new syntax_error("пропущено название " + lexical.get_pos());
            } else
                throw new syntax_error("программа не закрыта " + lexical.get_pos());
        }
        if (!start_lexma())
            throw new syntax_error("неизвестная стартовая лексема " + t.value + " " + lexical.get_pos());
    }
    throw new syntax_error("программа не закрыта " + lexical.get_pos());
}

bool syntax_analyzer::start_lexma() {
    token t;
    std::string out;
    if (lexma_is(INTEGER, t)) {
        if (lexma_is(TYPE, t)) {
            if (!initialization())
                throw new syntax_error("ошибка инициализации " + lexical.get_pos());
            return true;
        }
        if (lexma_is(IDENT, t)) {
            lexical.roll_back();
            if (!definition())
                throw new syntax_error("ошибка объявления " + lexical.get_pos());
            return true;
        }
    }
    if (lexma_is(IDENT, t)) {
        variable var;
        function func;
        std::list<token> poliz;
        if (funcs_table.get_by_name(t.value, func)) {
            lexical.roll_back();
            code_to_poliz(poliz);
            call_function(poliz, func);
            return true;
        }
        if (!idents_table->get_by_name(t.value, var))
            throw new semantic_error("переменная не объявлена " + lexical.get_pos());
        if (!lexma_is(EQUALS, t))
            throw new syntax_error("ожидалось = " + lexical.get_pos());
        var.initialized = true;
        if (!idents_table->set_by_name(var.name, var))
            throw new semantic_error("переменная не объявлена " + lexical.get_pos());
        code_to_poliz(poliz);
        output(poliz);
        out = "\tpopq\t%rax";
        code_gen.add_line(out);
        out = "\tmovq\t%rax, " + var.asm_name;
        code_gen.add_line(out);
        return true;
    }
    if (lexma_is(IF, t)) {
        lexical.roll_back();
        if (!if_statement())
            throw new syntax_error("ошибка условного оператора " + lexical.get_pos());
        return true;
    }
    if (lexma_is(DO, t)) {
        lexical.roll_back();
        if (!dowhile_statement())
            throw new syntax_error("ошибка цикла DO WHILE " + lexical.get_pos());
        return true;
    }
    if (lexma_is(PRINT, t)) {
        print();
        return true;
    }
    if (lexma_is(EXIT, t) && (opened_do == 0 || do_counter == 0))
        throw new syntax_error("EXIT вне цикла " + lexical.get_pos());
    if (lexma_is(CONTINUE, t) && (opened_do == 0 || do_counter == 0))
        throw new syntax_error("CONTINUE вне цикла ");
    return false;
}

bool syntax_analyzer::print() {
    std::string out;
    token t;
    std::list<token> poliz;
    do {
        code_to_poliz(poliz);
        output(poliz);
        out = "\tpopq\t%rsi";
        code_gen.add_line(out);
        out = "\tmovq\t$format, %rdi";
        code_gen.add_line(out);
        out = "\tmovq\t$0, %rax";
        code_gen.add_line(out);
        out = "\tcall\tprintf";
        code_gen.add_line(out);
        out = "\tmovq\t$0, %rax";
        code_gen.add_line(out);
    } while(lexma_is(COMMA, t));
    return true;
}

bool syntax_analyzer::dowhile_statement() {
    std::string out;
    token t;
    std::list<token> poliz;
    opened_do++;
    do_counter++;
    int this_counter = do_counter;
    if (!lexma_is(DO, t))
        throw new syntax_error("пропущен DO " + lexical.get_pos());
    if (!lexma_is(WHILE, t))
        throw new syntax_error("пропущен WHILE " + lexical.get_pos());
    if (!lexma_is(LBR, t))
        throw new syntax_error("пропущена ( " + lexical.get_pos());
    lexical.roll_back();
    code_to_poliz(poliz);
    out = "dowhile_label_" + std::to_string(this_counter) + ":";
    code_gen.add_line(out);
    output(poliz);
    out = "\tpopq\t%rax";
    code_gen.add_line(out);
    out = "\tcmpq\t$0, %rax";
    code_gen.add_line(out);
    out = "\tjle\t\tdowhile_exit_label_" + std::to_string(this_counter);
    code_gen.add_line(out);
    while(can_read(1)) {
        if (lexma_is(END, t)) {
            if (lexma_is(DO, t)) {
                out = "\tjmp\t\tdowhile_label_" + std::to_string(this_counter);
                code_gen.add_line(out);
                out = "dowhile_exit_label_" + std::to_string(this_counter) + ":";
                code_gen.add_line(out);
                opened_do--;
                return true;
            } else
                throw new syntax_error("пропущен END DO ");
        }
        if (lexma_is(EXIT, t)) {
            out = "\tjmp\t\tdowhile_exit_label_" + std::to_string(this_counter);
            code_gen.add_line(out);
            continue;
        }
        if (lexma_is(CONTINUE, t)) {
            out = "\tjmp\t\tdowhile_label_" + std::to_string(this_counter);
            code_gen.add_line(out);
            continue;
        }
        if (!start_lexma())
            throw new syntax_error("неизвестная стартовая лексема " + lexical.get_pos());
    }
    throw new syntax_error("ошибка цикла DO WHILE " + lexical.get_pos());
}

bool syntax_analyzer::if_statement() {
    token t;
    variable tmp;
    std::string out;
    std::list<token> poliz;
    if_counter++;
    int this_counter = if_counter;
    if (!lexma_is(IF, t))
        throw new syntax_error("пропущен IF " + lexical.get_pos());
    if (!lexma_is(LBR, t))
        throw new syntax_error("пропущена ( " + lexical.get_pos());
    lexical.roll_back();
    code_to_poliz(poliz);
    output(poliz);
    out = "\tpopq\t%rax";
    code_gen.add_line(out);
    out = "\tcmpq\t$0, %rax";
    code_gen.add_line(out);
    out = "\tje\t\tlabel_false_" + std::to_string(this_counter);
    code_gen.add_line(out);
    if (!lexma_is(THEN, t))
        throw new syntax_error("пропущен THEN " + lexical.get_pos());
    while(can_read(1)) {
        if (lexma_is(END, t)) {
            if (lexma_is(IF, t)) {
                out = "label_false_" + std::to_string(this_counter) + ":";
                code_gen.add_line(out);
                out = "if_exit_label_" + std::to_string(this_counter) + ":";
                code_gen.add_line(out);
                return true;
            } else
                throw new syntax_error("пропущен END IF " + lexical.get_pos());
        }
        if (lexma_is(ELSE, t)) {
            out = "\tjmp\t\tif_exit_label_" + std::to_string(this_counter);
            code_gen.add_line(out);
            break;
        }
        if (lexma_is(EXIT, t)) {
            out = "\tjmp\t\tdowhile_exit_label_" + std::to_string(do_counter);
            code_gen.add_line(out);
            continue;
        }
        if (lexma_is(CONTINUE, t)) {
            out = "\tjmp\t\tdowhile_label_" + std::to_string(do_counter);
            code_gen.add_line(out);
            continue;
        }
        if (!start_lexma())
            throw new syntax_error("неизвестная стартовая лексема " + lexical.get_pos());
    }
    out = "label_false_" + std::to_string(this_counter) + ":";
    code_gen.add_line(out);
    while(can_read(1)) {
        if (lexma_is(END, t)) {
            if (lexma_is(IF, t)) {
                out = "if_exit_label_" + std::to_string(this_counter) + ":";
                code_gen.add_line(out);
                return true;
            } else
                throw new syntax_error("пропущен IF " + lexical.get_pos());
        }
        if (lexma_is(EXIT, t)) {
            if (opened_do == 0 || do_counter == 0)
                throw new syntax_error("EXIT вне цикла " + lexical.get_pos());
            out = "\tjmp\t\tdowhile_exit_label_" + std::to_string(do_counter);
            code_gen.add_line(out);
            continue;
        }
        if (lexma_is(CONTINUE, t)) {
            if (opened_do == 0 || do_counter == 0)
                throw new syntax_error("CONTINUE вне цикла ");
            out = "\tjmp\t\tdowhile_label_" + std::to_string(do_counter);
            code_gen.add_line(out);
            continue;
        }
        if (!start_lexma())
            throw new syntax_error("неизвестная стартовая лексема " + lexical.get_pos());
    }
    throw new syntax_error("ошибка условного оператора " + lexical.get_pos());
}

bool syntax_analyzer::definition() {
    std::string out;
    token t;
    variable var;
    do {
        if (!lexma_is(IDENT, t))
            throw new syntax_error("пропущен IDENT " + lexical.get_pos());
        if (idents_table->get_by_name(t.value, var))
            throw new semantic_error("переменная уже объявлена " + lexical.get_pos());
        var.initialized = false;
        var.name = t.value;
        var.value = 0;
        rbp_vars_offset -= 8;
        var.asm_name = std::to_string(rbp_vars_offset) + "(%rbp)";
        out = "\tpushq\t$" + std::to_string(var.value);
        code_gen.add_line(out);
        idents_table->push(var);
        if (lexma_is(EQUALS, t))
            throw new syntax_error("= недопустимо " + lexical.get_pos());
    } while(lexma_is(COMMA, t));
    return true;
}

bool syntax_analyzer::initialization() {
    token t;
    variable var;
    std::string out;
    std::list<token> poliz;
    do {
        if (!lexma_is(IDENT, t))
            throw new syntax_error("пропущен IDENT " + lexical.get_pos());
        if (idents_table->get_by_name(t.value, var))
            throw new semantic_error("переменная уже объявлена " + lexical.get_pos());
        var.initialized = true;
        var.name = t.value;
        rbp_vars_offset -= 8;
        var.asm_name = std::to_string(rbp_vars_offset) + "(%rbp)";
        if (!lexma_is(EQUALS, t))
            throw new semantic_error("пропущено = " + lexical.get_pos());
        code_to_poliz(poliz);
        var.value = calculate_expression(poliz);
        out = "\tpushq\t$" + std::to_string(var.value);
        code_gen.add_line(out);
        idents_table->push(var);
    } while(lexma_is(COMMA, t));
    return true;
}

bool syntax_analyzer::can_read(int n) {
    token t;
    lexical.save_statement();
    for (int i = 0; i < n; i++) {
        if (!lexical.read_token(t)) {
            return false;
        }
    }
    lexical.roll_back();
    return true;
}

bool syntax_analyzer::lexma_is(token_type_enum type, token &t) {
    lexical.save_statement();
    if(!lexical.read_token(t))
        throw new syntax_error("нечего читать " + lexical.get_pos());
    if(t.type != type) {
        lexical.roll_back();
        return false;
    }
    return true;
}

bool syntax_analyzer::lexma_is(token_type_enum type, std::list<token> &lexems_list) {
    token t;
    lexical.save_statement();
    if(!lexical.read_token(t))
        throw new syntax_error("нечего читать " + lexical.get_pos());
    if(t.type != type) {
        lexical.roll_back();
        return false;
    }
    lexems_list.push_back(t);
    return true;
}

bool syntax_analyzer::on_expression(std::list<token> &list) {
    while(on_terminal(list)) {
        if(!(lexma_is(PLUS, list)           ||
             lexma_is(MINUS, list)          ||
             lexma_is(MUL, list)            ||
             lexma_is(DIV, list)            ||
             lexma_is(NOTEQUALS, list)      ||
             lexma_is(ISEQUALS, list)       ||
             lexma_is(LESSOREQUALS, list)   ||
             lexma_is(MOREOREQUALS, list)   ||
             lexma_is(AND, list)            ||
             lexma_is(OR, list)             ||
             lexma_is(LESS, list)           ||
             lexma_is(MORE, list)))
            return true;
    }
    return false;
}

bool syntax_analyzer::on_terminal(std::list<token> &lexems_list) {
    token t;
    if (lexma_is(LBR, lexems_list)) {
        if(!on_expression(lexems_list))
            return false;
        if (lexma_is(RBR, lexems_list))
            return true;
    } else {
        if (lexma_is(MINUS, t)) {
            t.value = '0';
            t.type = DIGIT;
            lexems_list.push_back(t);
            t.value = '-';
            t.type = MINUS;
            lexems_list.push_back(t);
        }
        if(lexma_is(IDENT, t)) {
            std::string call_name = t.value;
            if (lexma_is(LBR, t)) {
                std::list<token> params;
                function* func;
                calls_counter++;
                t.value = call_name + "_call_" + std::to_string(calls_counter);
                t.type = FUNCTION;
                lexems_list.push_back(t);
                int brkt_count = 1;
                while (lexical.read_token(t)) {
                    if (t.type == LBR)
                        brkt_count++;
                    if (t.type == RBR)
                        brkt_count--;
                    if (brkt_count == 0)
                        break;
                    params.push_back(t);
                }
                funcs_table.get_ptr_by_name(call_name, func);
                function_call fc(call_name + "_call_" + std::to_string(calls_counter), params, func);
                funcs_calls_table.push(fc);
                return true;
            }
            t.value = call_name;
            t.type = IDENT;
            lexems_list.push_back(t);
            return true;
        }
        if (lexma_is(DIGIT, lexems_list))
            return true;
    }
    return false;
}

bool syntax_analyzer::expression_to_tree(std::list<token>& list, tree_node<token>& root) {
    tree_node<token> *ptr1;
    tree_node<token> *ptr2;
    std::list<token>::iterator it = list.begin();
    std::list<token>::iterator min_it = list.begin();
    unsigned int size = list.size();
    if(size == 0) {
        return false;
    }
    if(size == 1) {
        root.value = list.back();
        return true;
    }
    bool flag = true;
    while(it != list.end()) {
        if (*it <= *min_it)
            min_it = it;
        if(it->type == LBR) {
            it++;
            int brkt_count = 1;
            while(brkt_count != 0) {
                if (it->type == LBR)
                    brkt_count++;
                if (it->type == RBR)
                    brkt_count--;
                it++;
            }
            if (list.front().type == LBR   &&
                list.back().type == RBR    &&
                it == list.end()           &&
                flag) {
                list.pop_back();
                list.pop_front();
                return expression_to_tree(list, root);
            }
        } else
            it++;
        flag = false;
    }
    root.value = *min_it;
    std::list<token> fst_branch(list.begin(), min_it--);
    std::list<token> scd_branch(++++min_it, list.end());
    ptr1 = new tree_node<token>;
    ptr2 = new tree_node<token>;
    root.children.push_back(ptr1);
    root.children.push_back(ptr2);
    if(!expression_to_tree(fst_branch, *ptr1)) return false;
    if(!expression_to_tree(scd_branch, *ptr2)) return false;
    return true;
}

int syntax_analyzer::calculate_expression(std::list<token> &poliz) {
    std::stack<int> stack;
    int r1, r2;
    variable var;
    std::list<token>::iterator it = poliz.begin();
    for(; it != poliz.end(); it++) {
        if(it->type == IDENT)
            throw new semantic_error("идентификатор в инициализации " + lexical.get_pos());
        if(it->type == DIGIT) {
            stack.push(atoi(it->value.c_str()));
        }
        if(it->type == PLUS         ||
           it->type == MINUS        ||
           it->type == MUL          ||
           it->type == DIV          ||
           it->type == NOTEQUALS    ||
           it->type == ISEQUALS     ||
           it->type == LESSOREQUALS ||
           it->type == MOREOREQUALS ||
           it->type == AND          ||
           it->type == OR           ||
           it->type == LESS         ||
           it->type == MORE) {
            if(stack.size() < 2)
                throw new semantic_error ("недостаточное число аргументов у оператора " + it->value);
            r2 = stack.top();
            stack.pop();
            r1 = stack.top();
            stack.pop();
            switch(it->type)
            {
                case PLUS:
                    r1 = r1 + r2;
                    break;
                case MINUS:
                    r1 = r1 - r2;
                    break;
                case MUL:
                    r1 = r1 * r2;
                    break;
                case DIV:
                    if(r2 == 0)
                        throw new semantic_error("ошибка деления на ноль");
                    r1 = r1 / r2;
                    break;
                case LESS:
                    r1 = r1 < r2;
                    break;
                case MORE:
                    r1 = r1 > r2;
                    break;
                case NOTEQUALS:
                    r1 = r1 != r2;
                    break;
                case ISEQUALS:
                    r1 = r1 == r2;
                    break;
                case LESSOREQUALS:
                    r1 = r1 <= r2;
                    break;
                case MOREOREQUALS:
                    r1 = r1 >= r2;
                    break;
                case AND:
                    r1 = r1 && r2;
                    break;
                case OR:
                    r1 = r1 || r2;
                    break;
            }
            stack.push(r1);
        }
    }
    if(stack.size() != 1)
        throw new semantic_error("ошибка вычисления арифметического выражения " + lexical.get_pos());
    return stack.top();
}

bool syntax_analyzer::output(std::list<token> &poliz) {
    std::list<token>::iterator it = poliz.begin();
    std::string out;
    variable var;
    function_call func;
    int this_counter;
    token t;
    while(it != poliz.end()) {
        if(it->type == FUNCTION) {
            if (!funcs_calls_table.get_by_name(it->value, func))
                throw new semantic_error("ошибка вызова функции " + it->value + " " + lexical.get_pos());
            if (!func.parent->returning)
                throw new semantic_error("функция не возвращает значений " + it->value + " " + lexical.get_pos());
            call_function(func.params, *(func.parent));
        }
        if(it->type == IDENT) {
            if (!idents_table->get_by_name(it->value, var))
                throw new semantic_error("нет объявления переменной " + it->value + " " + lexical.get_pos());
            if (!var.initialized)
                throw new semantic_error("нет инициализации переменной " + it->value + " " + lexical.get_pos());
            out = "\tmovq\t" + var.asm_name + ", %rax";
            code_gen.add_line(out);
            out = "\tpushq\t%rax";
            code_gen.add_line(out);
        }
        if(it->type == DIGIT) {
            out = "\tpushq\t$" + it->value;
            code_gen.add_line(out);
        }
        if(it->type == PLUS         ||
           it->type == MINUS        ||
           it->type == MUL          ||
           it->type == DIV          ||
           it->type == NOTEQUALS    ||
           it->type == ISEQUALS     ||
           it->type == LESSOREQUALS ||
           it->type == MOREOREQUALS ||
           it->type == AND          ||
           it->type == OR           ||
           it->type == LESS         ||
           it->type == MORE) {
            out = "\tpopq\t%rbx";
            code_gen.add_line(out);
            out = "\tpopq\t%rax";
            code_gen.add_line(out);
            switch(it->type)
            {
                case PLUS:
                    out = "\taddq\t%rbx, %rax";
                    code_gen.add_line(out);
                    break;
                case MINUS:
                    out = "\tsubq\t%rbx, %rax";
                    code_gen.add_line(out);
                    break;
                case MUL:
                    out = "\tmulq\t%rbx";
                    code_gen.add_line(out);
                    break;
                case DIV:
                    out = "\txorq\t%rdx, %rdx";
                    code_gen.add_line(out);
                    out = "\tdivq\t%rbx";
                    code_gen.add_line(out);
                    break;
                case LESS:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tjg\t\tcmp_less_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "\tjl\t\tcmp_less_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_less_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_less_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_less_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_less_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_less_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case MORE:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tjg\t\tcmp_more_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "\tjle\t\tcmp_more_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case NOTEQUALS:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tjne\t\tcmp_more_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "\tje\t\tcmp_more_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case ISEQUALS:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tje\t\tcmp_more_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "\tjne\t\tcmp_more_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case LESSOREQUALS:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tjle\t\tcmp_more_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "\tjg\t\tcmp_more_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case MOREOREQUALS:
                    cmp_counter++;
                    this_counter = cmp_counter;
                    out = "\tcmpq\t%rbx, %rax";
                    code_gen.add_line(out);
                    out = "\tjge\t\tcmp_more_true_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "jl\t\tcmp_more_false_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_true_" + std::to_string(this_counter) + ":\n\tmovq\t$1, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_false_" + std::to_string(this_counter) + ":\n\tmovq\t$0, %rax";
                    code_gen.add_line(out);
                    out = "\tjmp\t\tcmp_more_exit_" + std::to_string(this_counter);
                    code_gen.add_line(out);
                    out = "cmp_more_exit_" + std::to_string(this_counter) + ":";
                    code_gen.add_line(out);
                    break;
                case AND:
                    out = "\tandq\t%rbx, %rax";
                    code_gen.add_line(out);
                    break;
                case OR:
                    out = "\torq\t\t%rbx, %rax";
                    code_gen.add_line(out);
                    break;
            }
            out = "\tpushq\t%rax";
            code_gen.add_line(out);
        }
        it++;
    }
    return true;
}

bool syntax_analyzer::call_function(std::list<token> params, function func) {
    std::string out;
    token t;
    std::list<std::list<token>> args;
    std::list<token>::iterator it = params.begin();
    std::list<token>::iterator prev_it = params.begin();
    while (it != params.end()) {
        if (it->type == COMMA) {
            std::list<token> list(prev_it, it);
            args.push_back(list);
            prev_it = ++it;
        } else
            it++;
    }
    std::list<token> list(prev_it, it);
    args.push_back(list);
    args.reverse();
    std::list<std::list<token>>::iterator it1 = args.begin();
    it1 = args.begin();
    while (it1 != args.end()) {
        list_to_poliz(*it1);
        output(*it1);
        it1++;
    }
    out = "\tcall\t" + func.name;
    code_gen.add_line(out);
    out = "\taddq\t$" + std::to_string(func.arg_count * 8) + ", %rsp";
    code_gen.add_line(out);
    if (func.returning) {
        out = "\tpushq\t%rax";
        code_gen.add_line(out);
    }
    return true;
}

bool syntax_analyzer::function_declare(token t1) {
    std::string out;
    token t2;
    token result;
    variable var;
    function func;
    rbp_vars_offset = 0;
    rbp_params_offset = 8;
    if (idents_table != nullptr)
        delete idents_table;
    idents_table = new table<variable>;
    if (!lexma_is(IDENT, t1))
        throw new syntax_error("подпрограмма не названа " + lexical.get_pos());
    out = t1.value + ":";
    code_gen.add_line(out);
    out = "\tpushq\t%rbp";
    code_gen.add_line(out);
    out = "\tmovq\t%rsp, %rbp";
    code_gen.add_line(out);
    if (!lexma_is(LBR, t2))
        throw new syntax_error("пропущена ( " + lexical.get_pos());
    do {
        if (lexma_is(INTEGER, t2) || lexma_is(REAL, t2)) {
            if (!lexma_is(IDENT, t2))
                throw new syntax_error("пропущена переменная " + lexical.get_pos());
            var.value = 0;
            var.name = t2.value;
            rbp_params_offset += 8;
            var.asm_name = std::to_string(rbp_params_offset) + "(%rbp)";
            var.initialized = true;
            idents_table->push(var);
            func.arg_count++;
        }
    } while(lexma_is(COMMA, t2));
    if (!lexma_is(RBR, t2))
        throw new syntax_error("пропущена ) " + lexical.get_pos());
    if (lexma_is(RESULT, t2)) {
        if (!lexma_is(LBR, t2))
            throw new syntax_error("пропущена ) " + lexical.get_pos());
        if (!lexma_is(IDENT, t2))
            throw new syntax_error("пропущена возвращаемая переменная " + lexical.get_pos());
        result = t2;
        if (!lexma_is(RBR, t2))
            throw new syntax_error("пропущена ) " + lexical.get_pos());
        func.returning = true;
    }
    func.name = t1.value;
    funcs_table.push(func);
    function_statement(t2, result);
    if (t1.value != t2.value)
        throw new syntax_error("нет конца подпрограммы " + t1.value + " " + lexical.get_pos());
    return true;
}

bool syntax_analyzer::program_declare(token t1) {
    std::string out;
    token t2;
    rbp_vars_offset = 0;
    if (idents_table != nullptr)
        delete idents_table;
    idents_table = new table<variable>;
    if (!lexma_is(IDENT, t1))
        throw new syntax_error("программа не названа " + lexical.get_pos());
    out = "program:";
    code_gen.add_line(out);
    out = "\tpushq\t%rbp";
    code_gen.add_line(out);
    out = "\tmovq\t%rsp, %rbp";
    code_gen.add_line(out);
    program_statement(t2);
    if (t1.value != t2.value)
        throw new syntax_error("нет конца программы " + t1.value + " " + lexical.get_pos());
    return true;
}

bool syntax_analyzer::code_to_poliz(std::list<token> &poliz) {
    std::list<token> list;
    if(!on_expression(list))
        throw new semantic_error("ошибка получения списка лексем " + lexical.get_pos());
    expression_tree<token> tree;
    tree.root = new tree_node<token>;
    if(!expression_to_tree(list, *(tree.root)))
        throw new semantic_error("ошибка преобразование выражения " + lexical.get_pos());
    poliz = tree.reverse();
    return true;
}

bool syntax_analyzer::list_to_poliz(std::list<token> &poliz) {
    expression_tree<token> tree;
    tree.root = new tree_node<token>;
    if(!expression_to_tree(poliz, *(tree.root)))
        throw new semantic_error("ошибка преобразование выражения " + lexical.get_pos());
    poliz = tree.reverse();
    return true;
}

syntax_analyzer::~syntax_analyzer() {
    if (idents_table != nullptr)
        delete idents_table;
}
