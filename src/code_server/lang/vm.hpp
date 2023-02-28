#ifndef VM_H
#define VM_H

#include <memory>
#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/function.hpp"
#include "lang/error.hpp"

namespace discode {

class VM {

    Scope globals;
    Scope lib; // Todo: Generate library functions
    Data acc;
    std::vector<int> argument_stack;
    std::vector<FunctionPtr> function_stack;

    int ip = 0;
    int sp = -1;

    std::unique_ptr<discode::Error> err;

    void execute(std::vector<discode::Instruction> instructions)
    {
        while (ip < instructions.size())
        {
            discode::Instruction instruction = instructions[ip];

            ip++;

            if (instruction.opcode == discode::HALT)
            {
                return;
            }
            else if (instruction.opcode == discode::ICONST)
            {
                sp++;
                argument_stack.push_back(instruction.operands);
            }
            else if (instruction.opcode == discode::ADD)
            {
                int i = argument_stack[sp];
                sp--;
                int j = argument_stack[sp];
                argument_stack[sp] = i + j;
            }
        }
    }


};

};
#endif