#ifndef VM_H
#define VM_H

#include <memory>
#include <map>
#include <vector>

#include "lang/data.hpp"

namespace discode {

typedef std::map<std::string, Data> Scope;

class VM {

    Scope globals;
    Data acc;
    std::vector<Data> args;
    std::vector<Scope> stack;

public:
    
    void step();

};

};
#endif