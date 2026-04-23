/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include <iostream>

/* Implementation of the Statement class */

Statement::Statement() = default;

Statement::~Statement() = default;

/*
 * Implementation of RemStatement
 */
RemStatement::RemStatement(TokenScanner &scanner) {
    // Skip all tokens in the comment - do nothing
    while (scanner.hasMoreTokens()) {
        scanner.nextToken();
    }
}

RemStatement::~RemStatement() {
}

void RemStatement::execute(EvalState &state, Program &program) {
    // Do nothing for comments
}

/*
 * Implementation of LetStatement
 */
LetStatement::LetStatement(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

LetStatement::~LetStatement() {
    delete exp;
}

void LetStatement::execute(EvalState &state, Program &program) {
    exp->eval(state);
}

/*
 * Implementation of PrintStatement
 */
PrintStatement::PrintStatement(TokenScanner &scanner) {
    exp = parseExp(scanner);
}

PrintStatement::~PrintStatement() {
    delete exp;
}

void PrintStatement::execute(EvalState &state, Program &program) {
    int result = exp->eval(state);
    std::cout << result << std::endl;
}

/*
 * Implementation of InputStatement
 */
InputStatement::InputStatement(TokenScanner &scanner) {
    // According to spec: "10 INPUT 10" is valid, second 10 is variable name
    // Even though token scanner classifies numeric strings as NUMBER tokens,
    // we accept it as variable name for INPUT
    varName = scanner.nextToken();
}

InputStatement::~InputStatement() {
}

void InputStatement::execute(EvalState &state, Program &program) {
    int value;
    std::cin >> value;
    state.setValue(varName, value);
}

/*
 * Implementation of EndStatement
 */
EndStatement::EndStatement(TokenScanner &scanner) {
    // No arguments expected
}

EndStatement::~EndStatement() {
}

void EndStatement::execute(EvalState &state, Program &program) {
    throw ErrorException("END");
}

/*
 * Implementation of GotoStatement
 */
GotoStatement::GotoStatement(TokenScanner &scanner) {
    std::string token = scanner.nextToken();
    targetLine = stringToInteger(token);
}

GotoStatement::~GotoStatement() {
}

void GotoStatement::execute(EvalState &state, Program &program) {
    throw ErrorException("GOTO " + integerToString(targetLine));
}

/*
 * Implementation of IfStatement
 */
IfStatement::IfStatement(TokenScanner &scanner) {
    // Comparison operators have precedence 1, so we read with prec 2 to stop at operator
    exp1 = readE(scanner, 2);
    op = scanner.nextToken();
    exp2 = readE(scanner, 1);
    if (!scanner.hasMoreTokens()) {
        error("SYNTAX ERROR");
    }
    std::string thenToken = scanner.nextToken();
    if (thenToken != "THEN") {
        error("SYNTAX ERROR");
    }
    std::string lineToken = scanner.nextToken();
    targetLine = stringToInteger(lineToken);
}

IfStatement::~IfStatement() {
    delete exp1;
    delete exp2;
}

void IfStatement::execute(EvalState &state, Program &program) {
    int val1 = exp1->eval(state);
    int val2 = exp2->eval(state);
    bool condition = false;
    if (op == "=") condition = (val1 == val2);
    else if (op == "<") condition = (val1 < val2);
    else if (op == ">") condition = (val1 > val2);
    else if (op == "<=") condition = (val1 <= val2);
    else if (op == ">=") condition = (val1 >= val2);
    else if (op == "<>" || op == "!=") condition = (val1 != val2);
    else error("SYNTAX ERROR");

    if (condition) {
        throw ErrorException("GOTO " + integerToString(targetLine));
    }
}
