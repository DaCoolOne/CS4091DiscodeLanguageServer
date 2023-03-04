#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <memory>
#include <map>
#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/instruction.hpp"

namespace discode {

class Function : public Data
{
    std::vector<std::string> argnames;
    std::vector<std::shared_ptr<Instruction>> instructions;
public:
    Function(): Data(Type::TYPE_FUNCTION) { }
    Function(std::vector<std::string> &args, std::vector<std::shared_ptr<Instruction>> &ins): Data(Type::TYPE_FUNCTION), argnames(args), instructions(ins) { }

    std::shared_ptr<Data> copy() override { return std::make_shared<Function>(argnames, instructions); }

    void pushInstruction(Instruction * ins) { instructions.push_back(std::shared_ptr<Instruction>(ins)); }
    Instruction * get(int i) { return instructions.at(i).get(); }
    std::size_t length() { return instructions.size(); }
    const std::vector<std::string>& args() { return argnames; }

    std::string repr() override;
    std::string deepRepr(std::string ident = "") override;
};

class FunctionPtr
{
    Function * f_ptr;
    Scope local;
    uint32_t ins_cnt;
public:
    FunctionPtr(Function * fun, Scope &s): ins_cnt(0), f_ptr(fun), local(s) {}

    Scope * getLocal() { return &local; }
    Instruction * current() { return f_ptr->get(ins_cnt); }
    void gotoNext() { ++ins_cnt; }
    void gotoIndex(uint32_t index) { ins_cnt = index; }
    bool isComplete() { return ins_cnt >= f_ptr->length(); }

    std::string repr();
};

};
#endif