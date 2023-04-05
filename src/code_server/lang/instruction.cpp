

#include "lang/instruction.hpp"
#include "lang/function.hpp"
#include "lang/error.hpp"
#include "lang/vm.hpp"
#include "lib/lib_loader.hpp"

#include <iostream>
#include <limits>

bool discode::isTruthy(std::shared_ptr<discode::Data> data) {
    if (data->type == discode::Type::TYPE_NULL) { return false; }
    if (data->type == discode::Type::TYPE_BOOL && !data->getBool()) { return false; }
    return true;
}

void discode::InstructionPush::execute(discode::VM * vm) {
    vm->push(_data->copy());
}
std::string discode::InstructionPush::repr() {
    return "PUSH " + _data->repr();
}

void discode::InstructionPop::execute(discode::VM * vm) {
    vm->pop();
}
std::string discode::InstructionPop::repr() {
    return "POP";
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

void discode::InstructionReturn::execute(discode::VM * vm) {
    vm->popFunction();
}
std::string discode::InstructionReturn::repr() {
    return "RET";
}

void discode::InstructionUJump::execute(discode::VM * vm) {
    vm->jump(targetIndex);
}
std::string discode::InstructionUJump::repr() {
    return "UJMP " + std::to_string(targetIndex);
}

void discode::InstructionCJump::execute(discode::VM * vm) {
    std::shared_ptr<Data> jump_val = vm->pop();
    
    if (discode::isTruthy(jump_val)) {
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

    if (data->type == discode::TYPE_ARRAY) {
        if (name == "length") {
            vm->push(static_cast<double>(data->getVector()->size()));
            return;
        }
        else {
            vm->error(discode::ErrorKeyNotFound(name));
            return;
        }
    }

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
    std::shared_ptr<discode::Data> index = vm->pop();
    std::shared_ptr<discode::Data> data = vm->pop();

    if (data->type == discode::Type::TYPE_ARRAY) {
        if (index->type != discode::Type::TYPE_NUMBER) {
            vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_NUMBER, data->type));
            return;
        }

        int32_t int_index = static_cast<int32_t>(index->getNumber() + 0.5);

        auto vec = data->getVector();

        if (int_index < 0 || int_index >= vec->size()) {
            vm->error(discode::ErrorKeyNotFound(std::to_string(int_index)));
            return;
        }

        vm->push(vec->at(int_index));
    }
    else if(data->type == discode::Type::TYPE_OBJECT) {
        if (index->type != discode::Type::TYPE_STRING) {
            vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_STRING, data->type));
            return;
        }

        auto map = data->getMap();
        if (map->count(index->getString()) == 0) {
            vm->error(discode::ErrorKeyNotFound(index->getString()));
            return;
        }

        vm->push(map->at(index->getString()));
    }
    else {
        vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_OBJECT, data->type));
    }
}
std::string discode::InstructionGetIndexedStack::repr() {
    return "RESOLVE INDEX";
}

void discode::InstructionWriteLocal::execute(discode::VM * vm) {
    auto object = vm->pop();
    vm->writeLocal(ident, object);
}
std::string discode::InstructionWriteLocal::repr() {
    return "WRITE " + ident;
}

void discode::InstructionWriteLibrary::execute(discode::VM * vm) {
    // Todo: Lock libraries to read-only
}
std::string discode::InstructionWriteLibrary::repr() {
    return "WRITE !" + ident;
}

void discode::InstructionWriteGlobal::execute(discode::VM * vm) {
    // Todo: how to set globals? Maybe make this also read-only?
}
std::string discode::InstructionWriteGlobal::repr() {
    return "WRITE #" + ident;
}

void discode::InstructionWriteStack::execute(discode::VM * vm) {
    auto object = vm->pop();
    auto value = vm->pop();
    
    if (object->type != discode::Type::TYPE_OBJECT) {
        vm->error(discode::ErrorUnexpectedType(discode::Type::TYPE_OBJECT, object->type));
        return;
    }

    object->getMap()->insert_or_assign(ident, value);
}
std::string discode::InstructionWriteStack::repr() {
    return "WRITE STACK " + ident;
}

void discode::InstructionAdd::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type == Type::TYPE_NUMBER) {
        if (b->type != Type::TYPE_NUMBER) {
            vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
            return;
        }
        vm->push(a->getNumber() + b->getNumber());
        return;
    }
    if (a->type == Type::TYPE_STRING) {
        if (b->type != Type::TYPE_STRING) {
            vm->error(discode::ErrorUnexpectedType(Type::TYPE_STRING, b->type));
            return;
        }
        std::string res = a->getString() + b->getString();
        vm->push(res);
        return;
    }

    vm->error(discode::ErrorUnexpectedType({ Type::TYPE_NUMBER, Type::TYPE_STRING }, a->type));
}
std::string discode::InstructionAdd::repr() {
    return "ADD";
}

void discode::InstructionSub::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    vm->push(a->getNumber() - b->getNumber());
}
std::string discode::InstructionSub::repr() {
    return "SUB";
}

void discode::InstructionMul::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    vm->push(a->getNumber() * b->getNumber());
}
std::string discode::InstructionMul::repr() {
    return "MUL";
}

void discode::InstructionDivide::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    if (b->getNumber() == 0) {
        if (a->getNumber() >= 0) {
            vm->push(std::numeric_limits<double>::infinity());
        }
        else {
            vm->push(-std::numeric_limits<double>::infinity());
        }
    }
    else {
        vm->push(a->getNumber() / b->getNumber());
    }
}
std::string discode::InstructionDivide::repr() {
    return "DIV";
}

void discode::InstructionMod::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    if (b->getNumber() == 0) {
        if (a->getNumber() >= 0) {
            vm->push(std::numeric_limits<double>::infinity());
        }
        else {
            vm->push(-std::numeric_limits<double>::infinity());
        }
    }
    else {
        vm->push(a->getNumber() - b->getNumber() * static_cast<int>(a->getNumber() / b->getNumber()));
    }
}
std::string discode::InstructionMod::repr() {
    return "MOD";
}

void discode::InstructionAnd::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();
    vm->push(discode::isTruthy(a) && discode::isTruthy(b));
}
std::string discode::InstructionAnd::repr() {
    return "AND";
}

void discode::InstructionOr::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();
    vm->push(discode::isTruthy(a) || discode::isTruthy(b));
}
std::string discode::InstructionOr::repr() {
    return "OR";
}

void discode::InstructionNot::execute(discode::VM * vm) {
    vm->push(!discode::isTruthy(vm->pop()));
}
std::string discode::InstructionNot::repr() {
    return "NOT";
}

void discode::InstructionNeg::execute(discode::VM * vm) {
    auto a = vm->pop();
    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    vm->push(-a->getNumber());
}
std::string discode::InstructionNeg::repr() {
    return "NEG";
}

void discode::InstructionGtr::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    vm->push(a->getNumber() > b->getNumber());
}
std::string discode::InstructionGtr::repr() {
    return "GTR";
}

void discode::InstructionGteq::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, a->type));
        return;
    }
    if (b->type != Type::TYPE_NUMBER) {
        vm->error(discode::ErrorUnexpectedType(Type::TYPE_NUMBER, b->type));
        return;
    }

    vm->push(a->getNumber() >= b->getNumber());
}
std::string discode::InstructionGteq::repr() {
    return "GTEQ";
}

void discode::InstructionEq::execute(discode::VM * vm) {
    auto b = vm->pop();
    auto a = vm->pop();

    if (a->type != b->type) {
        vm->push(false);
        return;
    }
    
    switch(a->type) {
        case discode::Type::TYPE_NULL:
            vm->push(true);
        break;
        case discode::Type::TYPE_BOOL:
            vm->push(a->getBool() == b->getBool());
        break;
        case discode::Type::TYPE_NUMBER:
            vm->push(a->getNumber() == b->getNumber());
        break;
        case discode::Type::TYPE_STRING:
            vm->push(a->getString() == b->getString());
        break;
        default:
            vm->push(false);
        break;
    }
}
std::string discode::InstructionEq::repr() {
    return "EQ";
}

