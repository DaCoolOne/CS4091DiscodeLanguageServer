#ifndef FUNCTION_H
#define FUNCTION_H

#include <memory>
#include <map>
#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/instruction.hpp"

namespace discode {

// Note: I don't know if this is the right way to represent functions and function pointers. This is an educated guess.

class Function
{
    std::vector<std::string> argnames;
    std::vector<std::unique_ptr<Instruction>> instructions;
public:
    Function(): instructions() {}

    void pushInstruction(Instruction * ins) { instructions.push_back(std::unique_ptr<Instruction>(ins)); }
    Instruction * get(int i) { return instructions.at(i).get(); }
    std::size_t length() { return instructions.size(); }
};

class FunctionPtr
{
    std::shared_ptr<Function> f_ptr;
    Scope local;
    uint32_t ins_cnt;
public:
    FunctionPtr(Function * fun, Scope s): ins_cnt(0), f_ptr(fun), local(s) {}

    Scope * getLocal() { return &local; }
    Instruction * current() { return f_ptr->get(ins_cnt); }
    void gotoNext() { ++ins_cnt; }
    void gotoIndex(uint32_t index) { ins_cnt = index; }
    bool isComplete() { return ins_cnt >= f_ptr->length(); }
};

};
#endif