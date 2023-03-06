#ifndef VM_HPP
#define VM_HPP

#include <memory>
#include <vector>
#include <stdexcept>

#include "lang/data.hpp"
#include "lang/function.hpp"
#include "lang/error.hpp"

#include "socket/Socket.hpp"
#include "utils/json.hpp"

namespace discode {

#define MAX_FUNCTION_STACK_SIZE 1024

class VM {

    Scope globals;
    Scope lib;
    std::vector<std::shared_ptr<Data>> argument_stack;
    std::vector<FunctionPtr> function_stack;

    std::shared_ptr<discode::Error> err;

    Socket * _sock;

public:
    VM(Socket * sock);

    void push(std::shared_ptr<discode::Data> data);
    void pushNull();
    void push(bool boolean);
    void push(double num);
    void push(std::string &str);
    void push(std::vector<std::shared_ptr<Data>> &value);
    void push(std::map<std::string, std::shared_ptr<Data>> &value);

    std::shared_ptr<Data> pop();

    void error(Error error);
    void clearError();
    discode::Error * getError();

    void pushGlobal(std::string ident);
    void pushLocal(std::string ident);
    void pushLib(std::string ident);

    bool pushFunction(FunctionPtr &fptr);
    void popFunction();

    void jump(uint32_t newindex);

    void init(std::string fname, std::shared_ptr<Data> msg);
    void step();
    void run(uint16_t max_ins = 0);

    // Prints the vm's internal state
    void print();

    // Functions for loading instructions into the VM
    void clearAll();
    void writeGlobal(std::string channel, std::string data_name, std::shared_ptr<Data> data);
    void writeLib(std::string channel, std::string data_name, std::shared_ptr<Data> data);

    // Functions for sending stuff to the server
    void sendObject(json::JsonData * data);

};

};
#endif