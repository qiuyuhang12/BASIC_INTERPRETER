/*
 * File: exp.h
 * -----------
 * This interface defines a class hierarchy for expressions,
 * which allows the client to represent and manipulate simple
 * binary expression trees.
 */
#pragma once
#ifndef _exp_h
#define _exp_h

#include <string>
#include "Utils/error.hpp"
#include "evalstate.hpp"
#include "Utils/strlib.hpp"

/*
 * Type: ExpressionType
 * --------------------
 * This enumerated type is used to differentiate the three different
 * expression types: CONSTANT, IDENTIFIER, and COMPOUND.
 */

enum ExpressionType {
    CONSTANT, IDENTIFIER, COMPOUND
};

/*
 * Class: Expression
 * -----------------
 * This class is used to represent a node in an expression tree.
 * Expression is an example of an abstract class, which defines
 * the structure and behavior of a set of classes but has no
 * objects of its own.  Any object must be one of the three
 * concrete subclasses of Expression:
 *
 *  1. ConstantExp   -- an integer constant
 *  2. IdentifierExp -- a string representing an identifier
 *  3. CompoundExp   -- two expressions combined by an operator
 *
 * The Expression class defines the interface common to all
 * Expression objects; each subclass provides its own specific
 * implementation of the common interface.
 *
 * Note on syntax: Each of the virtual methods in the Expression
 * class is marked with the designation = 0 on the prototype line.
 * This notation is used in C++ to indicate that this method is
 * purely virtual and will always be supplied by the subclass.
 */

class Expression {

public:

/*
 * Constructor: Expression
 * -----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Expression();

/*
 * Destructor: ~Expression
 * Usage: delete exp;
 * ------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting an expression.
 */

    virtual ~Expression();

/*
 * Method: eval
 * Usage: int value = exp->eval(state);
 * ------------------------------------
 * Evaluates this expression and returns its value in the context of
 * the specified EvalState object.
 */

    virtual int eval(EvalState &state) = 0;

/*
 * Method: toString
 * Usage: string str = exp->toString();
 * ------------------------------------
 * Returns a string representation of this expression.
 */

    virtual std::string toString() = 0;

/*
 * Method: type
 * Usage: ExpressionType type = exp->getType();
 * --------------------------------------------
 * Returns the type of the expression, which must be one of the constants
 * CONSTANT, IDENTIFIER, or COMPOUND.
 */

    virtual ExpressionType getType() = 0;

    virtual std::string getName(std::string) = 0;
};

/*
 * Class: ConstantExp
 * ------------------
 * This subclass represents a constant integer expression.
 */

class ConstantExp : public Expression {

public:

/*
 * Constructor: ConstantExp
 * Usage: Expression *exp = new ConstantExp(value);
 * ------------------------------------------------
 * The constructor initializes a new integer constant expression
 * to the given value.
 */
    explicit ConstantExp() = default;

    explicit ConstantExp(int value);

    ~ConstantExp() override = default;

/*
 * Prototypes for the virtual methods
 * ----------------------------------
 * These methods have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

    std::string getName(std::string) override;

/*
 * Method: getValue
 * Usage: int value = ((ConstantExp *) exp)->getValue();
 * -----------------------------------------------------
 * Returns the value field without calling eval and can be applied
 * only to an object known to be a ConstantExp.
 */

    int getValue(int i);


    int value = 0;

};

/*
 * Class: IdentifierExp
 * --------------------
 * This subclass represents an expression corresponding to a variable.
 */

class IdentifierExp : public Expression {

public:

/*
 * Constructor: IdentifierExp
 * Usage: Expression *exp = new IdentifierExp(name);
 * -------------------------------------------------
 * The constructor initializes a new identifier expression
 * for the variable named by name.
 */
    explicit IdentifierExp() = default;

    explicit IdentifierExp(std::string name);

/*
 * Prototypes for the virtual methods
 * ----------------------------------
 * These methods have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

/*
 * Method: getName
 * Usage: string name = ((IdentifierExp *) exp)->getName();
 * --------------------------------------------------------
 * Returns the name field of the identifier node and can be applied only
 * to an object known to be an IdentifierExp.
 */

    std::string getName(std::string string) override;

    std::string name;

};

/*
 * Class: CompoundExp
 * ------------------
 * This subclass represents a compound expression consisting of
 * two subexpressions joined by an operator.
 */

class CompoundExp : public Expression {

public:

/*
 * Constructor: CompoundExp
 * Usage: Expression *exp = new CompoundExp(op, lhs, rhs);
 * -------------------------------------------------------
 * The constructor initializes a new compound expression
 * which is composed of the operator (op) and the left and
 * right subexpression (lhs and rhs).
 */
    CompoundExp() = default;

    CompoundExp(std::string op, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs);

/*
 * Prototypes for the virtual methods
 * ----------------------------------
 * These methods have the same prototypes as those in the Expression
 * base class and don't require additional documentation.
 */

    ~CompoundExp() override;

    int eval(EvalState &state) override;

    std::string toString() override;

    ExpressionType getType() override;

/*
 * Methods: getOp, getLHS, getRHS
 * Usage: string op = ((CompoundExp *) exp)->getOp();
 *        Expression *lhs = ((CompoundExp *) exp)->getLHS();
 *        Expression *rhs = ((CompoundExp *) exp)->getRHS();
 * ---------------------------------------------------------
 * These methods return the components of a compound node and can
 * be applied only to an object known to be a CompoundExp.
 */

    std::string getOp();

    std::shared_ptr<Expression> getLHS();

    std::shared_ptr<Expression> getRHS();

    std::string getName(std::string) override;

    std::string op;
    std::shared_ptr<Expression> lhs, rhs;

};

std::shared_ptr<Expression> formula(std::string fml);

#endif
