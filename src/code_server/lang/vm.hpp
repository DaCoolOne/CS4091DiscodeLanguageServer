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

public:

    void pushStack(FunctionPtr ptr) { stack.push_back(ptr); }
    void popStack() { stack.pop_back(); }

    void fcall(std::string name) { fcall(name, std::vector<std::string>()); }
    void fcall(std::string name, std::vector<std::string> args);

    bool resolve(NamedReference &ref, Data &out_data);
    void write(NamedReference &ref, Data data);

    void step();

    bool hasError() { return err.get() != nullptr; }
    void clearError() { err.reset(); }
    discode::Error * getError() { return err.get(); }
    bool isStopped() { return !stack.size(); }

    Scope * local() { return stack.back().getLocal(); }

    void throwError(std::string message);

    Error * run(std::string &name, std::vector<std::string> &args);

};

};
#endif