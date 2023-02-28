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
    //virtual void execute(discode::VM * vm) { throw std::logic_error("No execution specified"); }
    discode::OPCODE opcode;
    //std::vector<int> operands;
    int operands;
    
    public:
    // the args vector is an int simply to try and get things working for a basic example, I aim to 
        void execute(discode::OPCODE opcode, int args)//std::vector<int> args)
        {
            operands = args;
        }
};

// Place instructions here
};


#endif