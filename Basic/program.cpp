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
#include <map>
#include <vector>
#include <string>


Program::Program() = default;

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto& pair : lines) {
        delete pair.second.statement;
    }
    lines.clear();
    lineNumbers.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    if (lines.count(lineNumber)) {
        delete lines[lineNumber].statement;
    }
    lines[lineNumber] = Line(line);
    rebuildLineNumbers();
}

void Program::removeSourceLine(int lineNumber) {
    if (lines.count(lineNumber)) {
        delete lines[lineNumber].statement;
        lines.erase(lineNumber);
        rebuildLineNumbers();
    }
}

std::string Program::getSourceLine(int lineNumber) {
    if (lines.count(lineNumber)) {
        return lines[lineNumber].sourceLine;
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (!lines.count(lineNumber)) {
        error("Line " + integerToString(lineNumber) + " does not exist");
        return;
    }
    if (lines[lineNumber].statement != nullptr) {
        delete lines[lineNumber].statement;
    }
    lines[lineNumber].statement = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    if (!lines.count(lineNumber)) {
        return nullptr;
    }
    return lines[lineNumber].statement;
}

int Program::getFirstLineNumber() {
    if (lines.empty()) {
        return -1;
    }
    return lineNumbers[0];
}

int Program::getNextLineNumber(int lineNumber) {
    for (size_t i = 0; i <lineNumbers.size(); i++) {
        if (lineNumbers[i] == lineNumber && i + 1 < lineNumbers.size()) {
            return lineNumbers[i+1];
        }
    }
    return -1;
}


