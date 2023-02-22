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
};

// Place instructions here

};
#endif