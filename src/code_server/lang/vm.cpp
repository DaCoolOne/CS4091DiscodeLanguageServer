

#include "lang/vm.hpp"
#include <map>

void discode::VM::fcall(std::string name, std::vector<std::string> args) {
    if (stack.size()) { throw std::logic_error("Cannot call fcall with non-empty stack"); }

    // Create reference path
    NamedReference path(discode::ScopeType::SCOPE_GLOBAL, "command");
    path.appendRef(name);

    // Get function
    discode::Data data;
    if(!resolve(path, data)) return;
    // Todo: figure out error handling
    if(data.type != discode::Type::TYPE_FUNCTION) { throwError("Cannot call non-function"); return; }

    discode::Scope scope;

    // Load arguments
    for(int i = 0; i < args.size(); i++) {
        discode::Data temp(new std::string(args.at(i)));
        scope.insert(std::pair<std::string, discode::Data>("$a"+std::to_string(i), temp));
    }

    // Create the function pointer
    FunctionPtr f(static_cast<discode::Function*>(data.data.get()), scope);

    // Push the function onto the stack
    stack.push_back(f);
}

void discode::VM::step() {

    // Fetch instruction
    if (stack.size() == 0) {
        throwError("No code to run");
        return;
    }
    discode::Instruction * ins = stack.back().current();

    // Execute the instruction
    ins->execute(this);

    // Advance to next instruction
    if (stack.size()) { stack.back().gotoNext(); }

}

discode::Error * discode::VM::run(std::string &name, std::vector<std::string> &args) {
    fcall(name, args);

    while (!isStopped()) { step(); }

    if (hasError()) return err.get();
    return nullptr;
}

void discode::VM::throwError(std::string error_msg) {
    // Build the error message, unwinding the stack as we go.
    // Todo: use symbol tree to compute line numbers
    // Todo: Catch block to recover from stack unwind
    err.reset(new Error(error_msg));

    // Stack unwind
    while(stack.size()) {
        stack.pop_back();
        // Todo: Figure out which function was just popped
        // err->pushBt();
    }
}

bool discode::VM::resolve(discode::NamedReference &ref, discode::Data &out_data) {
    discode::Data data;
    bool success = false;
    switch (ref.scopeType())
    {
        case discode::ScopeType::SCOPE_GLOBAL: success = ref.resolve(&globals, out_data);
        case discode::ScopeType::SCOPE_LOCAL: success = ref.resolve(local(), out_data);
        case discode::ScopeType::SCOPE_LIBRARY: success = ref.resolve(&lib, out_data);
    }
    if (!success) {
        throwError("Could not find reference");
    }
    return success;
}

void discode::VM::write(discode::NamedReference &ref, discode::Data data) {
    bool success = false;
    discode::Data write_to;
    switch (ref.scopeType())
    {
        case discode::ScopeType::SCOPE_GLOBAL: success = ref.resolveOrMake(&globals, write_to);
        case discode::ScopeType::SCOPE_LOCAL: success = ref.resolveOrMake(local(), write_to);
        case discode::ScopeType::SCOPE_LIBRARY: success = ref.resolveOrMake(&lib, write_to);
    }
    
    if (!success) {
        throwError("Could not create reference");
        return;
    }

    write_to.type = data.type;
    switch (write_to.type)
    {
        case discode::Type::TYPE_NULL:
            write_to.data.reset();
        break;
        case discode::Type::TYPE_BOOL:
            write_to.data.reset(static_cast<bool*>(data.data.get()));
        break;
        case discode::Type::TYPE_NUMBER:
            write_to.data.reset(static_cast<double*>(data.data.get()));
        break;
        case discode::Type::TYPE_STRING:
            write_to.data.reset(static_cast<std::string*>(data.data.get()));
        break;
        case discode::Type::TYPE_ARRAY:
            write_to.data.reset(static_cast<std::vector<discode::Data>*>(data.data.get()));
        break;
        case discode::Type::TYPE_OBJECT:
            write_to.data.reset(static_cast<std::map<std::string, discode::Data>*>(data.data.get()));
        break;
        case discode::Type::TYPE_FUNCTION:
            write_to.data.reset(static_cast<Function*>(data.data.get()));
        break;
        case discode::Type::TYPE_METHOD:
            write_to.data.reset(static_cast<Function*>(data.data.get()));
        break;
    }
}
