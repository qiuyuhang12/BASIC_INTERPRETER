/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"

int find_cmp(const std::string &in) {
    int position = 0;
    while (position < in.size()) {
        char ch = in[position];
        if (ch == '=' || ch == '<' || ch == '>') {
            return position;
        }
        position++;
    }
    return -1;
}

std::shared_ptr<Expression> formula(std::string fml) {
    while (fml[0] == ' ') {
        fml = fml.substr(1);
    }
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(fml);
    return parseExp(scanner);
}

Program::Program(EvalState &state1) : state(state1) {}

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    //todo
    list.clear();
    run.clear();
}

void Program::addSourceLine(int lineNumber, std::string &line, commands cmd) {
//    if (lineNumber<0){
//        throw ErrorException("SYNTAX ERROR");
//    }

    // Replace this stub with your own code
    //todo
    if (list.find(lineNumber) != list.end()) {
        removeSourceLine(lineNumber);
    }
    list[lineNumber] = line;

    TokenScanner scanner2;
    scanner2.ignoreWhitespace();
    scanner2.scanNumbers();
    scanner2.setInput(line);
//    ???
    if (lineNumber != -1) {
        scanner2.nextToken();
    }
//    if (s=="-"){
//        scanner2.nextToken();
//    }
    scanner2.nextToken();
//    enum commands {REM,LET,PRINT,INPUT,END,GOTO,IF,RUN,LIST,CLEAR,QUIT,HELP};
    if (cmd == REM) {
        std::shared_ptr<Rem> tem = std::make_shared<Rem>();
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == LET) {
        line = line.substr(line.find('T') + 1);
        std::shared_ptr<Let> tem = std::make_shared<Let>();
        std::string fml = line.substr(line.find('=') + 2);
        tem->var.name = scanner2.nextToken();
        tem->val = formula(fml);
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == PRINT) {
        line = line.substr(line.find('T') + 1);
        std::shared_ptr<Print> tem = std::make_shared<Print>();
//        auto a=formula(line);
        tem->var = formula(line);
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == INPUT) {
        std::shared_ptr<Input> tem = std::make_shared<Input>();
        tem->var.name = scanner2.nextToken();
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == END) {
        std::shared_ptr<End> tem = std::make_shared<End>();
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == GOTO) {
        std::shared_ptr<Goto> tem = std::make_shared<Goto>();
        tem->line = stringToInteger(scanner2.nextToken());
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    } else if (cmd == IF) {
        int lhs_begin = line.find('F') + 1;
        int op_position = find_cmp(line);
        int rhs_end = line.find('T') - 1;
        std::string lhs = line.substr(lhs_begin, op_position - lhs_begin),
                rhs = line.substr(op_position + 1, rhs_end - op_position);
        std::shared_ptr<If> tem = std::make_shared<If>();
//        tem->lhs = scanner2.nextToken();

        tem->op = line[op_position];
//        tem->rhs = scanner2.nextToken();
        while (scanner2.nextToken()!="THEN"){
//            scanner2.nextToken();
        }
        tem->line_number = stringToInteger(scanner2.nextToken());
        scanner2.setInput(lhs);
        tem->lhs= parseExp(scanner2);
        scanner2.setInput(rhs);
        tem->rhs= parseExp(scanner2);
//        std::shared_ptr<Statement> exe=tem;
        run[lineNumber] = tem;
    }
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (list.find(lineNumber) == list.end()) {
//        throw ErrorException("LINE NUMBER ERROR");
        return;
    }
    list.erase(lineNumber);
    run.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (list.find(lineNumber) == list.end()) {
        error("LINE NUMBER ERROR");
    }
    return list[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo

}

//void Program::removeSourceLine(int lineNumber) {

std::shared_ptr<Statement> Program::getParsedStatement(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if (run.find(lineNumber) == run.end()) {
        throw ErrorException("LINE NUMBER ERROR");
    }
    return run[lineNumber];
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    return run.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    auto this_line = run.find(lineNumber);
    if (this_line == run.end()) {
        throw ErrorException("LINE NUMBER ERROR");
    }
    this_line++;
    if (this_line == run.end()) {
        end = true;
        return -1;
    }
    return this_line->first;
}

//more func to add
//todo
void Program::List() {
    for (const auto &line: list) {
        std::cout << line.second << '\n';
    }
}

void Program::Clear() {
    list.clear();
    run.clear();
}

void Program::Run() {
    if (list.empty()) {
        return;
    }
    end = false;
    line_now = getFirstLineNumber();
    while (!end) {
        getParsedStatement(line_now)->execute(state, *this);
    }
}

void Program::get_state(EvalState &state_in) {
    state = state_in;
}



