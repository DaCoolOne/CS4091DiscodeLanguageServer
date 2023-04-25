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

    std::map<Type, std::shared_ptr<Data>> builtin_map;

    Socket * _sock;

    std::string __server_id;

public:
    VM(Socket * sock, std::string server_id);

    std::string getServerId() { return __server_id; }

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

    std::shared_ptr<Data> getMethod(Type type, std::string name);

    void pushGlobal(std::string ident);
    void pushLocal(std::string ident);
    void pushLib(std::string ident);

    bool pushFunction(FunctionPtr &fptr);
    void popFunction();

    void jump(uint32_t newindex);

    void init(std::string fname, std::shared_ptr<Data> msg, json::JsonData * args);
    void step();
    void run(uint16_t max_ins = 0);

    // Prints the vm's internal state
    void print();
    void smallprint();

    // Functions for loading instructions into the VM
    void clearAll();
    void writeGlobal(std::string channel, std::string data_name, std::shared_ptr<Data> data);
    void writeLib(std::string channel, std::string data_name, std::shared_ptr<Data> data);

    void writeLocal(std::string ident, std::shared_ptr<Data> data);

    // Functions for sending stuff to the server
    void sendObject(json::JsonData * data);
    void sendError(discode::Error * err);
    void sendError(std::string err, std::string msg_id, std::string ch_id);

};

};
#endif