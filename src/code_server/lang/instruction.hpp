#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/scope.hpp"

namespace discode {

// Forward-declare the virtual machine.
class VM;

struct Instruction
{
    uint16_t linenum;
    Instruction(uint16_t lineno): linenum(lineno) {}

    virtual void execute(discode::VM * vm) { throw std::logic_error("No execution specified"); }
    virtual std::string repr() { throw std::logic_error("No representation of instruction"); }
    virtual uint32_t arrow(uint32_t curr) { throw std::logic_error("No arrow defined"); }
    virtual bool hasArrow() { return false; }
};

bool isTruthy(std::shared_ptr<discode::Data> data);

// Place instructions here
class InstructionPush : public Instruction
{
    std::shared_ptr<Data> _data;
public:
    InstructionPush(uint16_t lineno, std::shared_ptr<Data> data): Instruction(lineno), _data(data) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionPop : public Instruction
{
public:
    InstructionPop(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionFunctionCall : public Instruction
{
    uint16_t _num_args;
public:
    InstructionFunctionCall(uint16_t lineno, uint16_t num_args): Instruction(lineno), _num_args(num_args) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionMethodCall : public Instruction
{
    uint16_t _num_args;
    std::string method_name;
public:
    InstructionMethodCall(uint16_t lineno, uint16_t num_args, std::string name): Instruction(lineno), _num_args(num_args), method_name(name) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionReturn : public Instruction
{
public:
    InstructionReturn(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionUJump : public Instruction
{
    uint32_t targetIndex;
public:
    InstructionUJump(uint16_t lineno, uint32_t endpoint): Instruction(lineno), targetIndex(endpoint) { }

    void execute(discode::VM * vm);
    std::string repr();
    bool hasArrow() { return true; }
    uint32_t arrow(uint32_t index) { return targetIndex; }
};

class InstructionCJump : public Instruction
{
    uint32_t targetIndex;
public:
    InstructionCJump(uint16_t lineno, uint32_t endpoint): Instruction(lineno), targetIndex(endpoint) { }

    void execute(discode::VM * vm);
    std::string repr();
    bool hasArrow() { return true; }
    uint32_t arrow(uint32_t index) { return targetIndex; }
};

class InstructionIterCheck : public Instruction
{
    uint32_t targetIndex;
    std::string reg;
public:
    InstructionIterCheck(uint16_t lineno, std::string local, uint32_t endpoint): Instruction(lineno), reg(local), targetIndex(endpoint) { }

    void execute(discode::VM * vm);
    std::string repr();
    bool hasArrow() { return true; }
    uint32_t arrow(uint32_t index) { return targetIndex; }
};

class InstructionIterInc : public Instruction
{
    std::string reg;
public:
    InstructionIterInc(uint16_t lineno, std::string local): Instruction(lineno), reg(local) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetLocal : public Instruction
{
    std::string name;
public:
    InstructionGetLocal(uint16_t lineno, std::string identifier): Instruction(lineno), name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetGlobal : public Instruction
{
    std::string name;
public:
    InstructionGetGlobal(uint16_t lineno, std::string identifier): Instruction(lineno), name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetLib : public Instruction
{
    std::string name;
public:
    InstructionGetLib(uint16_t lineno, std::string identifier): Instruction(lineno), name(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetStack : public Instruction
{
    std::string name;
public:
    InstructionGetStack(uint16_t lineno, std::string indentifier): Instruction(lineno), name(indentifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGetIndexedStack : public Instruction
{
public:
    InstructionGetIndexedStack(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionWriteLocal : public Instruction
{
    std::string ident;
public:
    InstructionWriteLocal(uint16_t lineno, std::string identifier): Instruction(lineno), ident(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionWriteLibrary : public Instruction
{
    std::string ident;
public:
    InstructionWriteLibrary(uint16_t lineno, std::string identifier): Instruction(lineno), ident(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionWriteGlobal : public Instruction
{
    std::string ident;
public:
    InstructionWriteGlobal(uint16_t lineno, std::string identifier): Instruction(lineno), ident(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionWriteStack : public Instruction
{
    std::string ident;
public:
    InstructionWriteStack(uint16_t lineno, std::string identifier): Instruction(lineno), ident(identifier) { }

    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionAdd : public Instruction
{
public:
    InstructionAdd(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionSub : public Instruction
{
public:
    InstructionSub(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionMul : public Instruction
{
public:
    InstructionMul(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionDivide : public Instruction
{
public:
    InstructionDivide(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionMod : public Instruction
{
public:
    InstructionMod(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionAnd : public Instruction
{
public:
    InstructionAnd(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionOr : public Instruction
{
public:
    InstructionOr(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionNot : public Instruction
{
public:
    InstructionNot(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionNeg : public Instruction
{
public:
    InstructionNeg(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGtr : public Instruction
{
public:
    InstructionGtr(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionGteq : public Instruction
{
public:
    InstructionGteq(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

class InstructionEq : public Instruction
{
public:
    InstructionEq(uint16_t lineno): Instruction(lineno) {}
    void execute(discode::VM * vm);
    std::string repr();
};

};
#endif