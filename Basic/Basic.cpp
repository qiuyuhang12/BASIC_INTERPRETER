/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"
#include <string>

/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

void legal(const std::string &word);
int find_cmp(const std::string &in);
/* Main program */

int main() {
    EvalState state;
    Program program(state);
    //cout << "Stub implementation of BASIC" << endl;
//
//

//    std::string test="A)";
//////    int a= stringToInteger(test);
//////    std::cout<<a;
////////    int a=1+6+7+9*9+8/6;
////////    std::cin>>a;
////////    std::cout<<a;
//    TokenScanner s;
//    s.ignoreWhitespace();
//    s.scanNumbers();
//    s.setInput(test);
//    std::cout<<s.getTokenType(test);
////    SEPARATOR, WORD, NUMBER, STRING, OPERATOR
//    exit(0);
//    std::cout << (s.getTokenType("1+1+(1+1)"));
//    exit(0);
////    int a=test.find('6');
////    std::cout<<a<<'\n';
////    std::string d=test.substr(5);
////    std::cout<<d;
//    std::cout<<s.nextToken()<<std::endl;
//    std::cout<<s.nextToken()<<std::endl;
//    std::cout<<s.nextToken()<<std::endl;
//    std::cout<<s.nextToken()<<std::endl;
//    std::cout<<s.nextToken()<<std::endl;
////    std::cout<<s.getTokenType(s.nextToken());
//////    std::string u=s.getStringValue(test);
//////    std::cout<<test;
//////    std::cout<<s.nextToken()<<std::endl;
//////    std::cout<<s.nextToken()<<std::endl;
//////    s.nextToken();
////////    auto wha = s.nextToken();
//////    std::cout<<s.nextToken()<<std::endl;
//////    std::cout<<s.nextToken()<<std::endl;
//////    std::cout<<s.nextToken()<<std::endl;
//////    std::cout<<s.nextToken()<<std::endl;
//////    std::cout<<s.nextToken()<<std::endl;
//    exit(0);
//ConstantExp A;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    //todo
    std::string first_token = scanner.nextToken();
    if (scanner.getTokenType(first_token) == NUMBER) {
        int line_number = stringToInteger(first_token);
        if (line_number < 0) {
            throw ErrorException("SYNTAX ERROR");
        }
        std::string second_token = scanner.nextToken();
        if (second_token == "REM") {
            program.addSourceLine(line_number, line, REM);
        } else if (second_token == "LET") {
            std::string word=scanner.nextToken();
            if (scanner.getTokenType(word) != WORD || scanner.nextToken() != "=") {
                throw ErrorException("SYNTAX ERROR");
            }
            legal(word);
            program.addSourceLine(line_number, line, LET);
        } else if (second_token == "PRINT") {
//            std::string s = scanner.nextToken();
//            bool a = (scanner.getTokenType(s) != WORD && scanner.getTokenType(s) != NUMBER);
//            bool b = scanner.getTokenType(scanner.nextToken()) != TokenType(EOF);
//            if (a || b) {
//                throw ErrorException("SYNTAX ERROR");
//            }
            if (!scanner.hasMoreTokens()) {
                throw ErrorException("SYNTAX ERROR");
            }
            program.addSourceLine(line_number, line, PRINT);
        } else if (second_token == "INPUT") {
            std::string word=scanner.nextToken();
            legal(word);
            if (scanner.getTokenType(word) != WORD ||
                scanner.getTokenType(scanner.nextToken()) != TokenType(EOF)) {
                throw ErrorException("SYNTAX ERROR");
            }
            program.addSourceLine(line_number, line, INPUT);
        } else if (second_token == "END") {
            if (scanner.getTokenType(scanner.nextToken()) != TokenType(EOF)) {
                throw ErrorException("SYNTAX ERROR");
            }
            program.addSourceLine(line_number, line, END);
        } else if (second_token == "GOTO") {
            if (scanner.getTokenType(scanner.nextToken()) != NUMBER ||
                scanner.getTokenType(scanner.nextToken()) != TokenType(EOF)) {
                throw ErrorException("SYNTAX ERROR");
            }
            program.addSourceLine(line_number, line, GOTO);
        } else if (second_token == "IF") {
//            int cmp_begin=line.find('F')+1;
//            int cmp_end=line.find('T')-1;
//            std::string lhs;
//            std::string rhs;
            if (find_cmp(line)==-1){
                throw ErrorException("SYNTAX ERROR");
            }
            bool find_then= false,find_line= false,no_ap= false;
            while (scanner.hasMoreTokens()){
                if (scanner.nextToken()=="THEN"){
                    find_then= true;
                    if (scanner.getTokenType(scanner.nextToken())==NUMBER){
                        find_line= true;
                        if (!scanner.hasMoreTokens()){
                            no_ap= true;
                            break;
                        }
                    }
                }
            }
            if(!(find_then&&find_line&&no_ap)){
                throw ErrorException("SYNTAX ERROR");
            }
//            while ()
//            std::string t3 = scanner.nextToken();// x
//            std::string t4 = scanner.nextToken();// <
//            std::string t5 = scanner.nextToken();// y
//            std::string t6 = scanner.nextToken();//then
//            std::string t7 = scanner.nextToken();//9
//            std::string t8 = scanner.nextToken();
//            if ((scanner.getTokenType(t3) != WORD && scanner.getTokenType(t3) != NUMBER) ||
//                (t4 != "<" && t4 != ">" && t4 != "=") ||
//                (scanner.getTokenType(t5) != WORD && scanner.getTokenType(t5) != NUMBER) || t6 != "THEN" ||
//                scanner.getTokenType(t7) != NUMBER || !t8.empty()) {
//                throw ErrorException("SYNTAX ERROR");
//            }
            program.addSourceLine(line_number, line, IF);
        } else if (second_token.empty()) {
            program.removeSourceLine(line_number);
        } else throw ErrorException("SYNTAX ERROR");
    } else {
//        少了一堆
        if (first_token == "RUN") {
            if (scanner.hasMoreTokens()){
                throw ErrorException("SYNTAX ERROR");
            }
            program.Run();
        } else if (first_token == "LIST") {
            if (scanner.hasMoreTokens()){
                throw ErrorException("SYNTAX ERROR");
            }
            program.List();
        } else if (first_token == "CLEAR") {
            if (scanner.hasMoreTokens()){
                throw ErrorException("SYNTAX ERROR");
            }
            program.Clear();
            state.Clear();
        } else if (first_token == "QUIT") {
            if (scanner.hasMoreTokens()){
                throw ErrorException("SYNTAX ERROR");
            }
            exit(0);
        } else if (first_token == "HELP") {
            if (!scanner.nextToken().empty()) {
                throw ErrorException("SYNTAX ERROR");
            }
            std::cout << "Yet another basic interpreter\n";
        } else if (first_token == "LET") {
//            if (scanner.getTokenType(scanner.nextToken()) != WORD || scanner.nextToken() != "=" ||
//                scanner.getTokenType(scanner.nextToken()) != NUMBER ||
//                scanner.getTokenType(scanner.nextToken()) != TokenType(EOF)) {
//                throw ErrorException("SYNTAX ERROR");
//            }
            std::string word=scanner.nextToken();
            legal(word);
            if (scanner.getTokenType(word) != WORD || scanner.nextToken() != "=") {
                throw ErrorException("SYNTAX ERROR");
            }
//            program.addSourceLine(line_number, line, LET);
            program.addSourceLine(-1, line, LET);
            program.getParsedStatement(-1)->execute(state, program);
            program.line_now = -1;
            program.removeSourceLine(-1);

        } else if (first_token == "PRINT") {
//            if (scanner.getTokenType(scanner.nextToken()) != WORD ||
//                scanner.getTokenType(scanner.nextToken()) != TokenType(EOF)) {
//                throw ErrorException("SYNTAX ERROR");
//            }
            if (!scanner.hasMoreTokens()) {
                throw ErrorException("SYNTAX ERROR");
            }
//            program.addSourceLine(line_number, line, PRINT);

            program.addSourceLine(-1, line, PRINT);
//            std::cout<<std::endl;
            program.getParsedStatement(-1)->execute(state, program);
            program.line_now = -1;
//            program.Run();
//            std::cout<<std::endl;
            program.removeSourceLine(-1);
        } else if (first_token == "INPUT") {
            std::string word=scanner.nextToken();
            legal(word);
            bool a = scanner.getTokenType(word) != WORD, b = !scanner.nextToken().empty();
            if (a || b) {
                throw ErrorException("SYNTAX ERROR");
            }
            program.addSourceLine(-1, line, INPUT);
            program.getParsedStatement(-1)->execute(state, program);
            program.line_now = -1;
            program.removeSourceLine(-1);
        } else throw ErrorException("SYNTAX ERROR");
    }
}

void legal(const std::string &word) {
    if (word == "REM" || word == "LET" || word == "PRINT" || word == "INPUT" || word == "END" || word == "GOTO" ||
        word == "IF" || word == "THEN" || word == "RUN" || word == "LIST" || word == "CLEAR" || word == "QUIT" ||
        word == "HELP") {
        error("SYNTAX ERROR");
    }
}
//int find_cmp(const std::string &in){
//    int position=0;
//    while (position<in.size()){
//        char ch=in[position];
//        if (ch=='='||ch=='<'||ch=='>'){
//            return position;
//        }
//        position++;
//    }
//    return -1;
//}