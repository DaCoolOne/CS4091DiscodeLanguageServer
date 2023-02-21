#ifndef VM_H
#define VM_H

#include <memory>
#include <vector>

#include "lang/data.hpp"
#include "lang/function.hpp"
#include "lang/error.hpp"

namespace discode {

class VM {

    Scope globals;
    Scope lib; // Todo: Generate library functions
    Data acc;
    std::vector<Data> args;
    std::vector<FunctionPtr> stack;

    std::unique_ptr<discode::Error> err;

    bool halted = true;

};

};
#endif