/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

void Rem::execute(EvalState &state, Program &program) {
    program.line_now = program.getNextLineNumber(program.line_now);
}


void Let::execute(EvalState &state, Program &program) {
    program.line_now = program.getNextLineNumber(program.line_now);
    state.setValue(var.name, val->eval(state));
}


void Print::execute(EvalState &state, Program &program) {
    std::cout << var->eval(state) << std::endl;
    program.line_now = program.getNextLineNumber(program.line_now);
}


void Input::execute(EvalState &state, Program &program) {
    while (true) {
        std::cout << " ? ";
        std::string tem;
        getline(std::cin, tem);
        TokenScanner t;
        t.ignoreWhitespace();
        t.scanNumbers();
        t.setInput(tem);
        std::string first = t.nextToken();
        if (first != "-") {
            if (t.hasMoreTokens()) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            if (t.getTokenType(first) != NUMBER) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            if (first.find('.') < first.size()) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            state.setValue(var.toString(), stringToInteger(tem));
        } else {
            if (!t.hasMoreTokens()) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            std::string second = t.nextToken();
            if (t.hasMoreTokens()) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            if (t.getTokenType(second) != NUMBER) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            if (second.find('.') < second.size()) {
                std::cout << ("INVALID NUMBER\n");
                continue;
            }
            state.setValue(var.toString(), -stringToInteger(second));
        }
        program.line_now = program.getNextLineNumber(program.line_now);
        break;
    }
}


void End::execute(EvalState &state, Program &program) {
    program.end = true;
}


void Goto::execute(EvalState &state, Program &program) {
    program.line_now = line;
}


void If::execute(EvalState &state, Program &program) {
    int left, right;
    TokenScanner t;
    left = lhs->eval(state);
    right = rhs->eval(state);
    bool cmp = false;
    if ((op == "=" && left == right) || (op == "<" && left < right) || (op == ">" && left > right))cmp = true;
    if (cmp) {
        program.line_now = line_number;
    } else {
        program.line_now = program.getNextLineNumber(program.line_now);
    }
}
