#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/scope.hpp"

namespace discode {

// Forward-declare the virtual machine
class VM;

struct Instruction
{
    virtual void execute(discode::VM * vm) { throw std::logic_error("No execution specified"); }
    virtual std::string repr() { throw std::logic_error("No representation of instruction"); }
    virtual uint32_t arrow(uint32_t curr) { throw std::logic_error("No arrow defined"); }
    virtual bool hasArrow() { return false; }
};

// Place instructions here
class InstructionPush : public Instruction
{
    std::shared_ptr<Data> _data;
public:
    InstructionPush(std::shared_ptr<Data> data): _data(data) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionFunctionCall : public Instruction
{
    uint16_t _num_args;
public:
    InstructionFunctionCall(uint16_t num_args): _num_args(num_args) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionUJump : public Instruction
{
    uint32_t targetIndex;
public:
    InstructionUJump(uint32_t endpoint): targetIndex(endpoint) { }

    void execute(discode::VM * vm);
    std::string repr();
    bool hasArrow() { return true; }
    uint32_t arrow(uint32_t index) { return targetIndex; }
};

class InstructionCJump : public Instruction
{
    uint32_t targetIndex;
public:
    InstructionCJump(uint32_t endpoint): targetIndex(endpoint) { }

    void execute(discode::VM * vm);
    std::string repr();
    bool hasArrow() { return true; }
    uint32_t arrow(uint32_t index) { return targetIndex; }
};

class InstructionGetLocal : public Instruction
{
    std::string name;
public:
    InstructionGetLocal(std::string identifier): name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetGlobal : public Instruction
{
    std::string name;
public:
    InstructionGetGlobal(std::string identifier): name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetLib : public Instruction
{
    std::string name;
public:
    InstructionGetLib(std::string identifier): name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetStack : public Instruction
{
    std::string name;
public:
    InstructionGetStack(std::string indentifier): name(indentifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetIndexedStack : public Instruction
{
public:
    InstructionGetIndexedStack() { }

    void execute(discode::VM * vm);
    std::string repr();
};

};
#endif