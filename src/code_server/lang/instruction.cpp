

#include "lang/instruction.hpp"
#include "lang/vm.hpp"

#include <iostream>
#include <limits>

void discode::InstructionBinaryNum::execute(discode::VM * vm)
{
    // Get two numbers, execute an operation, and result in destination.
    discode::Data arg1, arg2;
    if(!vm->resolve(_arg1, arg1) || !vm->resolve(_arg2, arg2)) return;

    // Assert that both sides are numbers
    if (arg1.type != discode::Type::TYPE_NUMBER || arg2.type != discode::Type::TYPE_NUMBER) {
        vm->throwError("Cannot perform math operation on non-number type");
        return;
    }

    double a = *static_cast<double*>(arg1.data.get());
    double b = *static_cast<double*>(arg2.data.get());

    double res;
    switch (_type)
    {
        case discode::BinaryNumInstructionType::BIN_ALU_INS_ADD:
            res = a + b;
        break;
        case discode::BinaryNumInstructionType::BIN_ALU_INS_SUB:
            res = a - b;
        break;
        case discode::BinaryNumInstructionType::BIN_ALU_INS_MUL:
            res = a * b;
        break;
        case discode::BinaryNumInstructionType::BIN_ALU_INS_DIV:
            if (b == 0) {
                res = std::numeric_limits<double>::infinity() * (a >= 0 ? 1 : -1);
            }
            else {
                res = a / b;
            }
        break;
    }

    vm->write(_dest, discode::Data(new double(res)));

}

void discode::InstructionDebugPrint::execute(discode::VM * vm)
{
    discode::Data arg;
    if(!vm->resolve(_arg, arg)) return;

    switch (arg.type)
    {
        case discode::Type::TYPE_NULL:
            std::cout << "null" << std::endl;
        break;
        case discode::Type::TYPE_BOOL:
            std::cout << (*static_cast<bool*>(arg.data.get()) ? "true" : "false") << std::endl;
        break;
        case discode::Type::TYPE_NUMBER:
            std::cout << *static_cast<double*>(arg.data.get()) << std::endl;
        break;
        case discode::Type::TYPE_STRING:
            std::cout << *static_cast<std::string*>(arg.data.get()) << std::endl;
        break;
    
        case discode::Type::TYPE_ARRAY:
            std::cout << "[ array ]" << std::endl;
        break;
        case discode::Type::TYPE_OBJECT:
            std::cout << "{ object }" << std::endl;
        break;
        case discode::Type::TYPE_FUNCTION:
            std::cout << "function()" << std::endl;
        break;
        case discode::Type::TYPE_METHOD:
            std::cout << "method()" << std::endl;
        break;
    }
}

void discode::InstructionEQ::execute(discode::VM * vm)
{
    discode::Data arg1, arg2;
    if(!vm->resolve(_arg1, arg1) || !vm->resolve(_arg2, arg2)) return;
    
    bool res;

    if(arg1.type != arg2.type)
    {
        res = false;
    }
    else
    {
        switch (arg1.type)
        {
            case discode::Type::TYPE_NULL:
                res = true;
            break;
            case discode::Type::TYPE_BOOL:
                res = *static_cast<bool*>(arg1.data.get()) == *static_cast<bool*>(arg1.data.get());
            break;
            case discode::Type::TYPE_NUMBER:
                res = *static_cast<double*>(arg1.data.get()) == *static_cast<double*>(arg1.data.get());
            break;
            case discode::Type::TYPE_STRING:
                res = (static_cast<std::string*>(arg1.data.get())->compare(*static_cast<std::string*>(arg1.data.get()))) == 0;
            break;

            default:
                res = arg1.data.get() == arg2.data.get();
        }
    }

    vm->write(_dest, discode::Data(new bool(res)));
}

void discode::InstructionReturn::execute(discode::VM * vm)
{
    discode::Data ret;
    if (hasRet) {
        if(!vm->resolve(_ret, ret)) return;
    }

    vm->popStack();

    if (hasRet) {
        discode::NamedReference rpath(discode::ScopeType::SCOPE_LOCAL, "$ret");
        vm->write(rpath, ret);
    }
}


void discode::InstructionNumCompare::execute(discode::VM * vm)
{
    // Get two numbers, execute an operation, and result in destination.
    discode::Data arg1, arg2;
    if(!vm->resolve(_arg1, arg1) || !vm->resolve(_arg2, arg2)) return;

    // Assert that both sides are numbers
    if (arg1.type != discode::Type::TYPE_NUMBER || arg2.type != discode::Type::TYPE_NUMBER) {
        vm->throwError("Cannot perform math operation on non-number type");
        return;
    }

    double a = *static_cast<double*>(arg1.data.get());
    double b = *static_cast<double*>(arg2.data.get());

    bool res;
    switch (_type)
    {
        case discode::InstructionNumCompareType::COMP_ALU_INS_GTEQ:
            res = a >= b;
        break;
        case discode::InstructionNumCompareType::COMP_ALU_INS_GTR:
            res = a > b;
        break;
    }

    vm->write(_dest, discode::Data(new bool(res)));

}

