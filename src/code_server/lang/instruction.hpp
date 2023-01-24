#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>

#include "lang/data.hpp"
#include "lang/scope.hpp"

namespace discode {

// Forward-declare the virtual machine
class VM;

struct Instruction
{
    virtual void execute(discode::VM * vm) { throw std::logic_error("No execution specified"); }
};

enum BinaryNumInstructionType
{
    BIN_ALU_INS_ADD,
    BIN_ALU_INS_SUB,
    BIN_ALU_INS_MUL,
    BIN_ALU_INS_DIV
};

enum InstructionNumCompareType
{
    COMP_ALU_INS_GTR,
    COMP_ALU_INS_GTEQ,
};

class InstructionBinaryNum : Instruction
{
    NamedReference _arg1;
    NamedReference _arg2;
    NamedReference _dest;

    BinaryNumInstructionType _type;
public:
    InstructionBinaryNum(BinaryNumInstructionType type, NamedReference dest, NamedReference arg1, NamedReference arg2):
        _type(type), _dest(dest), _arg1(arg1), _arg2(arg2) {}
    
    void execute(discode::VM * vm);
};

class InstructionEQ : Instruction
{
    NamedReference _arg1;
    NamedReference _arg2;
    NamedReference _dest;
public:
    InstructionEQ(NamedReference dest, NamedReference arg1, NamedReference arg2): _dest(dest), _arg1(arg1), _arg2(arg2) {}

    void execute(discode::VM * vm);
};

class InstructionNumCompare : Instruction
{
    NamedReference _arg1;
    NamedReference _arg2;
    NamedReference _dest;

    InstructionNumCompareType _type;
public:
    InstructionNumCompare(NamedReference dest, NamedReference arg1, NamedReference arg2): _dest(dest), _arg1(arg1), _arg2(arg2) {}

    void execute(discode::VM * vm);
};

class InstructionReturn : Instruction
{
    bool hasRet;
    NamedReference _ret;
public:
    InstructionReturn(): hasRet(false), _ret(discode::ScopeType::SCOPE_LOCAL, "") {}
    InstructionReturn(NamedReference returnValue): hasRet(true), _ret(returnValue) {}

    void execute(discode::VM * vm);
};

class InstructionDebugPrint : Instruction
{
    NamedReference _arg;
public:
    InstructionDebugPrint(NamedReference arg): _arg(arg) {}

    void execute(discode::VM * vm);
};

};
#endif