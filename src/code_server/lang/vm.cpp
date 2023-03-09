

#include "lang/vm.hpp"
#include "lib/libs.hpp"
#include <map>
#include <stdexcept>
#include <iostream>
#include "utils/json.hpp"

#include <chrono>
#include <thread>

void discode::VM::init(std::string fname, std::shared_ptr<discode::Data> msg)
{
    if(globals.count("commands") == 0 || globals.at("commands")->type != Type::TYPE_OBJECT || globals.at("commands")->getMap()->count(fname) == 0) {
        error(discode::Error("Could not find #commands." + fname));
        return;
    }

    std::shared_ptr<discode::Data> obj = globals.at("commands")->getMap()->at(fname);
    if(obj->type != Type::TYPE_FUNCTION) {
        error(discode::ErrorUnexpectedType(Type::TYPE_FUNCTION, obj->type));
        return;
    }

    discode::Function * function = dynamic_cast<discode::Function *>(obj.get());

    if(function->args().size() != 1) {
        error(discode::ErrorBadArgumentCount(function->args().size()));
        return;
    }
    Scope locals;
    locals.insert(std::pair<std::string, std::shared_ptr<discode::Data>>(function->args().at(0), msg));
    
    argument_stack.clear();
    function_stack.clear();

    discode::FunctionPtr fptr(function, locals);
    pushFunction(fptr);
}

void discode::VM::step()
{
    if (function_stack.empty())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return;
    }

    discode::FunctionPtr &active = function_stack.back();
    if (active.isComplete())
    {
        popFunction();
        return;
    }
    
    discode::Instruction * ins = active.current();
    active.gotoNext();
    ins->execute(this);
}

void discode::VM::run(uint16_t max_ins)
{
    print();
    while(!function_stack.empty() && getError() == nullptr) {
        step();
        print();
        --max_ins;
        if(max_ins == 0) {
            error(discode::InfiniteLoopError());
            break;
        }
    }
    // Todo: proper error handling
    if (getError() != nullptr) {
    }
    // Clean up the stacks for future use
    function_stack.clear();
    argument_stack.clear();
}

void discode::VM::push(std::shared_ptr<discode::Data> data)
{
    argument_stack.push_back(data);
}

void discode::VM::pushNull()
{
    push(std::make_shared<discode::Null>());
}

void discode::VM::push(bool boolean)
{
    push(std::make_shared<discode::Boolean>(boolean));
}

void discode::VM::push(double num)
{
    push(std::make_shared<discode::Number>(num));
}

void discode::VM::push(std::string &str)
{
    push(std::make_shared<discode::String>(str));
}

void discode::VM::push(std::vector<std::shared_ptr<discode::Data>> &value)
{
    push(std::make_shared<discode::Array>(value));
}

void discode::VM::push(std::map<std::string, std::shared_ptr<discode::Data>> &value)
{
    push(std::make_shared<discode::Object>(value));
}

std::shared_ptr<discode::Data> discode::VM::pop()
{
    if(argument_stack.empty())
    {
        throw std::logic_error("Attempt to pop empty stack");
    }

    std::shared_ptr<discode::Data> arg = argument_stack.back();
    argument_stack.pop_back();
    return arg;
}

void discode::VM::error(discode::Error error)
{
    err = std::make_shared<discode::Error>(error);
}

void discode::VM::clearError()
{
    err.reset(static_cast<discode::Error*>(nullptr));
}

discode::Error * discode::VM::getError()
{
    return err.get();
}

void discode::VM::pushGlobal(std::string ident)
{
    if (globals.count(ident) == 0) {
        error(discode::ErrorNotFound(ident));
        return;
    }
    argument_stack.push_back(globals.at(ident));
}

void discode::VM::pushLocal(std::string ident)
{
    Scope * local = function_stack.back().getLocal();
    if (local->count(ident) == 0) {
        error(discode::ErrorNotFound(ident));
        return;
    }
    argument_stack.push_back(local->at(ident));
}

void discode::VM::writeLocal(std::string ident, std::shared_ptr<discode::Data> data)
{
    Scope * local = function_stack.back().getLocal();
    local->insert(std::pair<std::string, std::shared_ptr<discode::Data>>(ident, data));
}

void discode::VM::pushLib(std::string ident)
{
    if (lib.count(ident) == 0) {
        error(discode::ErrorNotFound(ident));
        return;
    }
    argument_stack.push_back(lib.at(ident));
}

bool discode::VM::pushFunction(discode::FunctionPtr &fptr)
{
    if(function_stack.size() >= MAX_FUNCTION_STACK_SIZE)
    {
        error(discode::ErrorFunctionStackFull());
        return false;
    }

    function_stack.push_back(fptr);
    return true;
}

void discode::VM::popFunction()
{
    if(function_stack.empty())
    {
        throw std::logic_error("Attempt to pop empty function stack");
    }

    function_stack.pop_back();
} 

void discode::VM::jump(uint32_t index)
{
    if (function_stack.empty())
    {
        throw std::logic_error("Unable to jump with empty function stack");
    }
    
    discode::FunctionPtr active = function_stack.back();
    active.gotoIndex(index);
}

void discode::VM::clearAll()
{
    argument_stack.clear();
    function_stack.clear();
    globals.clear();
    lib.clear();
    clearError();
}

void discode::VM::writeGlobal(std::string channel, std::string data_name, std::shared_ptr<discode::Data> data)
{
    // Create channel scope if it doesn't yet exist
    if (!globals.count(channel) || globals.at(channel)->type != discode::TYPE_OBJECT)
    {
        globals.insert(std::pair<std::string, std::shared_ptr<discode::Data>>(channel, std::make_shared<discode::Object>()));
    }
    discode::Scope * channelObject = globals.at(channel)->getMap();
    channelObject->insert(std::pair<std::string, std::shared_ptr<discode::Data>>(data_name, data));
}

void discode::VM::writeLib(std::string channel, std::string data_name, std::shared_ptr<discode::Data> data)
{
    // Create channel scope if it doesn't yet exist
    if (!lib.count(channel) || lib.at(channel)->type != discode::TYPE_OBJECT)
    {
        lib.insert(std::pair<std::string, std::shared_ptr<discode::Data>>(channel, std::make_shared<discode::Object>()));
    }
    discode::Scope * channelObject = lib.at(channel)->getMap();
    channelObject->insert(std::pair<std::string, std::shared_ptr<discode::Data>>(data_name, data));
}

// Helper function
void printScope(discode::Scope scope)
{
    for(auto const it : scope)
    {
        std::cout << "[" << it.first << "] -> " << it.second->deepRepr() << std::endl;
    }
}

void discode::VM::print()
{
    std::cout << " --------- ARG STACK ---------" << std::endl;
    for(uint16_t i = 0; i < argument_stack.size(); ++i) {
        std::cout << argument_stack.at(i)->repr() << std::endl;
    }
    std::cout << " --------- RET STACK ---------" << std::endl;
    for(uint16_t i = 0; i < function_stack.size(); ++i) {
        std::cout << function_stack.at(i).repr() << std::endl;
    }
    std::cout << " --------- LIB SCOPE ---------" << std::endl;
    printScope(lib);
    std::cout << " --------- GLB SCOPE ---------" << std::endl;
    printScope(globals);
    std::cout << " --------- ERR STATE ---------" << std::endl;
    if (err != nullptr) {
        std::cout << err->what() << std::endl;
    }
}

discode::VM::VM(Socket * sock): _sock(sock)
{
    lib::loadAll(this);
}

void discode::VM::sendObject(json::JsonData * data)
{
    _sock->SendLine(data->toJsonString());
}
