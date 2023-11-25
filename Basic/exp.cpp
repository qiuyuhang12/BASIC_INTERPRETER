/*
 * File: exp.cpp
 * -------------
 * This file implements the Expression class and its subclasses.
 */

#include "exp.hpp"


/*
 * Implementation notes: the Expression class
 * ------------------------------------------
 * The Expression class declares no instance variables and needs no code.
 */
//std::shared_ptr<Expression> formula(std::string fml) {
//
//    while (fml[0] == ' ') {
//        fml = fml.substr(1);
//    }
//    TokenScanner scanner;
//    scanner.ignoreWhitespace();
//    scanner.scanNumbers();
//    scanner.setInput(fml);
//    return parseExp(scanner);
//    std::string first = scanner.nextToken();
//    if (scanner.getTokenType(first) == NUMBER && !scanner.hasMoreTokens()) {
//        std::shared_ptr<ConstantExp> tem=std::make_shared<ConstantExp>();
//        tem->value=stringToInteger(first);
//        return tem;
//    }
//    if (scanner.getTokenType(first) == WORD && !scanner.hasMoreTokens()) {
//        std::shared_ptr<IdentifierExp> tem=std::make_shared<IdentifierExp>();
//        tem->name=(first);
//        return tem;
//    }
//
////    std::shared_ptr<CompoundExp> exp;
//    if (first == "(") {
//        int num = 1;
//        int position = 1;
//        while (num > 0) {
//            if (position >= fml.size()) {
//                error("SYNTAX ERROR");
//            }
//            if (fml[position] == '(') {
//                num += 1;
//            } else if (fml[position] == ')') {
//                num -= 1;
//            }
//            position += 1;
//        }
//        std::string lhs_string = fml.substr(1, position - 2);
////        if (position > fml.size() || position == fml.size() - 1) {
////            error("SYNTAX ERROR");
////        }
//        std::string op_with_rhs = fml.substr(position);
//        TokenScanner scanner_rhs;
//        scanner_rhs.ignoreWhitespace();
//        scanner_rhs.scanNumbers();
//        scanner_rhs.setInput(op_with_rhs);
//
//        if (!scanner_rhs.hasMoreTokens()) {
//            return formula(lhs_string);
//        }
//        std::string op = scanner_rhs.nextToken();
//        if (op != "+" && op != "-" && op != "*" && op != "/") {
//            error("SYNTAX ERROR");
//        }
//        if (!scanner_rhs.hasMoreTokens()) {
//            error("SYNTAX ERROR");
//        }
//        std::string rhs_string = fml.substr(position + 1);
//        std::shared_ptr<CompoundExp> exp1=std::make_shared<CompoundExp>();
////        ???
//        exp1->op = op;
//        exp1->lhs = formula(lhs_string);
//        exp1->rhs = formula(rhs_string);
//        return exp1;
////        CompoundExp a (op, formula(lhs_string),formula(rhs_string));
//    } else if (scanner.getTokenType(first) == WORD || scanner.getTokenType(first) == NUMBER) {
//        std::string op = scanner.nextToken();
//        if (op != "+" && op != "-" && op != "*" && op != "/") {
//            error("SYNTAX ERROR");
//        }
//        if (!scanner.hasMoreTokens()) {
//            error("SYNTAX ERROR");
//        }
//        char op_char = op[0];
//        int op_position = fml.find(op_char);
//        std::string rhs = fml.substr(op_position + 1);
//        while (rhs[0] == ' ') {
//            rhs = rhs.substr(1);
//        }
//        std::shared_ptr<CompoundExp> exp1=std::make_shared<CompoundExp>();
//        exp1->lhs = formula(first);
//        exp1->rhs = formula(rhs);
//        exp1->op = op;
//        return exp1;
//    }
//    error("SYNTAX ERROR");
//}

Expression::Expression() = default;

Expression::~Expression() = default;

/*
 * Implementation notes: the ConstantExp subclass
 * ----------------------------------------------
 * The ConstantExp subclass declares a single instance variable that
 * stores the value of the constant.  The eval method doesn't use the
 * value of state but needs it to match the general prototype for eval.
 */

ConstantExp::ConstantExp(int value) {
    this->value = value;
}

std::string ConstantExp::getName(std::string a) {
    return a;
}

int ConstantExp::eval(EvalState &state) {
    return value;
}

std::string ConstantExp::toString() {
    return integerToString(value);
}

ExpressionType ConstantExp::getType() {
    return CONSTANT;
}

int ConstantExp::getValue(int i) {
    return value;
}

/*
 * Implementation notes: the IdentifierExp subclass
 * ------------------------------------------------
 * The IdentifierExp subclass declares a single instance variable that
 * stores the name of the variable.  The implementation of eval must
 * look this variable up in the evaluation state.
 */

IdentifierExp::IdentifierExp(std::string name) {
    this->name = name;
}

int IdentifierExp::eval(EvalState &state) {
    if (!state.isDefined(name)) error("VARIABLE NOT DEFINED");
    return state.getValue(name);
}

std::string IdentifierExp::toString() {
    return name;
}

ExpressionType IdentifierExp::getType() {
    return IDENTIFIER;
}

std::string IdentifierExp::getName(std::string string) {
    return name;
}

/*
 * Implementation notes: the CompoundExp subclass
 * ----------------------------------------------
 * The CompoundExp subclass declares instance variables for the operator
 * and the left and right subexpressions.  The implementation of eval 
 * evaluates the subexpressions recursively and then applies the operator.
 */

CompoundExp::CompoundExp(std::string op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs) {
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

CompoundExp::~CompoundExp() {
    //delete lhs;
    //delete rhs;
}

/*
 * Implementation notes: eval
 * --------------------------
 * The eval method for the compound expression case must check for the
 * assignment operator as a special case.  Unlike the arithmetic operators
 * the assignment operator does not evaluate its left operand.
 */

int CompoundExp::eval(EvalState &state) {
    if (op == "=") {
        if (lhs->getType() != IDENTIFIER) {
            error("Illegal variable in assignment");
        }
        if (lhs->getType() == IDENTIFIER && lhs->toString() == "LET")
            error("SYNTAX ERROR");
        int val = rhs->eval(state);
//        state.setValue(((IdentifierExp *) lhs)->getName(std::string()), val);
        state.setValue((lhs)->getName(std::string()), val);
        return val;
    }
    int left = lhs->eval(state);
    int right = rhs->eval(state);
    if (op == "+") return left + right;
    if (op == "-") return left - right;
    if (op == "*") return left * right;
    if (op == "/") {
        if (right == 0) error("DIVIDE BY ZERO");
        return left / right;
    }
    return 0;
}

std::string CompoundExp::toString() {
    return '(' + lhs->toString() + ' ' + op + ' ' + rhs->toString() + ')';
}

ExpressionType CompoundExp::getType() {
    return COMPOUND;
}

std::string CompoundExp::getOp() {
    return op;
}

std::shared_ptr<Expression> CompoundExp::getLHS() {
    return lhs;
}

std::shared_ptr<Expression> CompoundExp::getRHS() {
    return rhs;
}

std::string CompoundExp::getName(std::string a) {
    return a;
}
