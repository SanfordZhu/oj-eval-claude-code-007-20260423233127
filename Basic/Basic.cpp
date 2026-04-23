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


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
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

bool isKeyword(const std::string& token) {
    static const std::set<std::string> keywords = {
        "REM", "LET", "PRINT", "INPUT", "END", "GOTO", "IF", "THEN",
        "RUN", "LIST", "CLEAR", "QUIT", "HELP"
    };
    return keywords.count(token) > 0;
}

Statement *parseStatement(const std::string& commandName, TokenScanner &scanner) {
    if (commandName == "REM") return new RemStatement(scanner);
    else if (commandName == "LET") return new LetStatement(scanner);
    else if (commandName == "PRINT") return new PrintStatement(scanner);
    else if (commandName == "INPUT") return new InputStatement(scanner);
    else if (commandName == "END") return new EndStatement(scanner);
    else if (commandName == "GOTO") return new GotoStatement(scanner);
    else if (commandName == "IF") return new IfStatement(scanner);
    else error("SYNTAX ERROR");
    return nullptr;
}

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    if (!scanner.hasMoreTokens()) {
        return;
    }

    std::string firstToken = scanner.nextToken();
    TokenType type = scanner.getTokenType(firstToken);

    // Check if line starts with a number = stored program line
    if (type == NUMBER) {
        int lineNumber = stringToInteger(firstToken);
        if (!scanner.hasMoreTokens()) {
            // Empty line after number = delete line
            program.removeSourceLine(lineNumber);
            return;
        }
        // Add/replace source line
        program.addSourceLine(lineNumber, line);
        // Parse the statement starting from next token
        TokenScanner lineScanner;
        lineScanner.ignoreWhitespace();
        lineScanner.scanNumbers();
        lineScanner.setInput(line);
        // Skip the line number token
        lineScanner.nextToken();
        std::string command = lineScanner.nextToken();
        Statement *stmt = parseStatement(command, lineScanner);
        program.setParsedStatement(lineNumber, stmt);
        return;
    }

    // Direct command (immediate execution)
    std::string command = firstToken;

    if (command == "RUN") {
        // Execute program starting from first line
        int currentLine = program.getFirstLineNumber();
        while (currentLine != -1) {
            Statement *stmt = program.getParsedStatement(currentLine);
            if (stmt == nullptr) {
                currentLine = program.getNextLineNumber(currentLine);
                continue;
            }
            try {
                stmt->execute(state, program);
                currentLine = program.getNextLineNumber(currentLine);
            } catch (ErrorException &ex) {
                std::string msg = ex.getMessage();
                if (msg == "END") {
                    break;
                } else if (msg.substr(0, 5) == "GOTO ") {
                    int target = stringToInteger(msg.substr(5));
                    if (program.getSourceLine(target).empty()) {
                        error("LINE NOT FOUND");
                    }
                    currentLine = target;
                } else {
                    throw;
                }
            }
        }
        return;
    } else if (command == "LIST") {
        // List all program lines in order
        int currentLine = program.getFirstLineNumber();
        while (currentLine != -1) {
            std::cout << program.getSourceLine(currentLine) << std::endl;
            currentLine = program.getNextLineNumber(currentLine);
        }
        return;
    } else if (command == "CLEAR") {
        program.clear();
        state.Clear();
        return;
    } else if (command == "QUIT") {
        exit(0);
    } else if (command == "HELP") {
        std::cout << "BASIC Interpreter - Type RUN to execute, LIST to list, CLEAR to clear, QUIT to exit" << std::endl;
        return;
    } else if (isKeyword(command)) {
        // Direct execution of statement
        Statement *stmt = parseStatement(command, scanner);
        try {
            stmt->execute(state, program);
            delete stmt;
        } catch (ErrorException &ex) {
            delete stmt;
            throw;
        }
        return;
    } else {
        error("SYNTAX ERROR");
    }
}

