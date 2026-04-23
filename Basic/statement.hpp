/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program) = 0;

};


/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

/*
 * Class: RemStatement
 * -------------------
 * Represents a REM comment statement (does nothing).
 */
class RemStatement : public Statement {
public:
    RemStatement(TokenScanner &scanner);
    virtual ~RemStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
};

/*
 * Class: LetStatement
 * -------------------
 * Represents a LET assignment statement.
 */
class LetStatement : public Statement {
public:
    LetStatement(TokenScanner &scanner);
    virtual ~LetStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
private:
    Expression *exp;
};

/*
 * Class: PrintStatement
 * ---------------------
 * Represents a PRINT statement that evaluates and prints an expression.
 */
class PrintStatement : public Statement {
public:
    PrintStatement(TokenScanner &scanner);
    virtual ~PrintStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
private:
    Expression *exp;
};

/*
 * Class: InputStatement
 * ----------------------
 * Represents an INPUT statement that reads a value into a variable.
 */
class InputStatement : public Statement {
public:
    InputStatement(TokenScanner &scanner);
    virtual ~InputStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
private:
    std::string varName;
};

/*
 * Class: EndStatement
 * --------------------
 * Represents an END statement that terminates program execution.
 */
class EndStatement : public Statement {
public:
    EndStatement(TokenScanner &scanner);
    virtual ~EndStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
};

/*
 * Class: GotoStatement
 * ---------------------
 * Represents a GOTO statement that jumps to a specific line number.
 */
class GotoStatement : public Statement {
public:
    GotoStatement(TokenScanner &scanner);
    virtual ~GotoStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
private:
    int targetLine;
};

/*
 * Class: IfStatement
 * ------------------
 * Represents an IF-THEN conditional branch statement.
 */
class IfStatement : public Statement {
public:
    IfStatement(TokenScanner &scanner);
    virtual ~IfStatement() override;
    virtual void execute(EvalState &state, Program &program) override;
private:
    Expression *exp1;
    std::string op;
    Expression *exp2;
    int targetLine;
};

#endif
