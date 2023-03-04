

#include "lang/instruction.hpp"
#include "lang/function.hpp"
#include "lang/error.hpp"
#include "lang/vm.hpp"
#include "lib/lib_loader.hpp"

#include <iostream>
#include <limits>

// Implement instructions here
void discode::InstructionPush::execute(discode::VM * vm) {
    vm->push(_data->copy());
}
std::string discode::InstructionPush::repr() {
    return "PUSH " + _data->repr();
}

void discode::InstructionFunctionCall::execute(discode::VM * vm) {
    std::shared_ptr<Data> fdata = vm->pop();
    std::vector<std::shared_ptr<Data>> args;
    for(uint16_t i = 0; i < _num_args; ++i)
    {
        args.push_back(vm->pop());
    }
    // Check function type
    // Todo: method calls
    if (fdata->type == discode::Type::TYPE_FUNCTION)
    {
        Function * function = dynamic_cast<Function*>(fdata.get());

        // Make sure that function expects the number of arguments we have.
        if (function->args().size() != args.size())
        {
            vm->error(discode::ErrorBadArgumentCount(function->args().size()));
            return;
        }

        // Assemble the local scope
        discode::Scope locals;
        for(uint16_t i = 0; i < _num_args; ++i)
        {
            locals.insert(std::pair<std::string, std::shared_ptr<discode::Data>>(function->args().at(i), args.at(i)));
        }

        // Push function ptr to stack
        FunctionPtr fptr(function, locals);
        vm->pushFunction(fptr); // discard return since we don't use it
    }
    // Library function hooks
    else if(fdata->type == discode::Type::TYPE_INTERNAL_FUNCTION)
    {
        lib::LibFunction * function = dynamic_cast<lib::LibFunction*>(fdata.get());

        function->call(vm, args);
    }
    else
    {
        vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_FUNCTION, fdata->type)); return;
    }
}
std::string discode::InstructionFunctionCall::repr() {
    return "FCALL " + std::to_string(_num_args);
}

void discode::InstructionUJump::execute(discode::VM * vm) {
    vm->jump(targetIndex);
}
std::string discode::InstructionUJump::repr() {
    return "UJMP " + std::to_string(targetIndex);
}

void discode::InstructionCJump::execute(discode::VM * vm) {
    std::shared_ptr<Data> jump_val = vm->pop();
    
    if (jump_val->type != TYPE_NULL && (jump_val->type != TYPE_BOOL || jump_val->getBool())) {
        vm->jump(targetIndex);
    }
}
std::string discode::InstructionCJump::repr() {
    return "CJMP " + std::to_string(targetIndex);
}

void discode::InstructionGetLocal::execute(discode::VM * vm) {
    vm->pushLocal(name);
}
std::string discode::InstructionGetLocal::repr() {
    return "GET " + name;
}

void discode::InstructionGetGlobal::execute(discode::VM * vm) {
    vm->pushGlobal(name);
}
std::string discode::InstructionGetGlobal::repr() {
    return "GET #" + name;
}

void discode::InstructionGetLib::execute(discode::VM * vm) {
    vm->pushLib(name);
}
std::string discode::InstructionGetLib::repr() {
    return "GET !" + name;
}

void discode::InstructionGetStack::execute(discode::VM * vm) {
    std::shared_ptr<discode::Data> data = vm->pop();

    if (data->type != discode::TYPE_OBJECT) {
        vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_OBJECT, data->type));
        return;
    }

    auto map = data->getMap();
    if (map->count(name) == 0) {
        vm->error(discode::ErrorKeyNotFound(name));
        return;
    }

    vm->push(map->at(name));
}
std::string discode::InstructionGetStack::repr() {
    return "RESOLVE ." + name;
}

void discode::InstructionGetIndexedStack::execute(discode::VM * vm) {
    
}
std::string discode::InstructionGetIndexedStack::repr() {
    return "RESOLVE INDEX";
}
