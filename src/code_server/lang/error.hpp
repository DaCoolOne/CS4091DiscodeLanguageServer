#ifndef ERROR_HPP
#define ERROR_HPP

#include <vector>
#include <string>
#include <stdexcept>
#include "lang/scope.hpp"
#include "utils/string.hpp"

namespace discode
{

class Error
{
    std::string message;
    std::vector<discode::NamedReference> backtrace;
public:
    Error(std::string msg): message(msg) {}

    void pushBt(discode::NamedReference bt) { backtrace.push_back(bt); }

    std::string what() {
        std::vector<std::string> lines;
        lines.reserve(backtrace.size() + 1);
        lines.push_back(message);
        for (int i = 0; i < backtrace.size(); i ++) {
            lines.push_back("\tin " + backtrace.at(i).stringRepr());
        }
        return util::join(lines, '\n');
    }
};

class ErrorBadArgumentCount : public Error
{
public:
    ErrorBadArgumentCount(uint16_t expected, uint16_t got): Error("Incorrect number of argument in function call! Expected " + std::to_string(expected) + " got " + std::to_string(got)) {}
};

class ErrorFunctionStackFull : public Error
{
public:
    ErrorFunctionStackFull(): Error("Stack overflow! Potential infinite recursion.") {}
};

class ErrorNotFound : public Error
{
public:
    ErrorNotFound(std::string identifier): Error("Could not find name " + identifier + " in scope!") {}
};

class ErrorKeyNotFound : public Error
{
public:
    ErrorKeyNotFound(std::string identifier): Error("Could not find " + identifier + " in object!") {}
};

class ErrorUnexpectedType : public Error
{
public:
    ErrorUnexpectedType(Type expected, Type got): Error("Unexpected type " + getTypeStr(got) + ", expected "  + getTypeStr(expected)) { }
    ErrorUnexpectedType(std::vector<Type> expected, Type got): Error("Unexpected type " + getTypeStr(got) + ", expected "  + getTypeStr(expected)) { }
};

class InfiniteLoopError : public Error
{
public:
    InfiniteLoopError(): Error("Code took too long to run.") { }
};

};
#endif