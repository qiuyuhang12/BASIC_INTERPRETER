/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include "parser.hpp"


/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

std::shared_ptr<Expression> parseExp(TokenScanner &scanner) {
    std::shared_ptr<Expression> exp = readE(scanner);
    if (scanner.hasMoreTokens()) {
        error("parseExp: Found extra token: " + scanner.nextToken());
    }
    return exp;
}

/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

std::shared_ptr<Expression> readE(TokenScanner &scanner, int prec) {
    std::shared_ptr<Expression> exp = readT(scanner);
    std::string token;
    while (true) {
        token = scanner.nextToken();
        int newPrec = precedence(token);
        if (newPrec <= prec) break;
        std::shared_ptr<Expression> rhs = readE(scanner, newPrec);
        exp = std::make_shared<CompoundExp>(token, exp, rhs);
//??
    }
    scanner.saveToken(token);
    return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

std::shared_ptr<Expression> readT(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    TokenType type = scanner.getTokenType(token);
    if (type == WORD) return std::make_shared<IdentifierExp>(token);
    if (type == NUMBER) return std::make_shared<ConstantExp>(stringToInteger(token));
    if (token == "-") return std::make_shared<CompoundExp>(token, std::make_shared<ConstantExp>(0), readE(scanner));
//??
    if (token != "(") error("Illegal term in expression");
    std::shared_ptr<Expression> exp = readE(scanner);
    if (scanner.nextToken() != ")") {
        error("Unbalanced parentheses in expression");
    }
    return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(std::string token) {
    if (token == "=") return 1;
    if (token == "+" || token == "-") return 2;
    if (token == "*" || token == "/") return 3;
    return 0;
}
