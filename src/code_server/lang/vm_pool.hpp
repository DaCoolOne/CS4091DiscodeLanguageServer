#ifndef VM_POOL_HPP
#define VM_POOL_HPP

#include <map>
#include <string>
#include <memory>

#include "lang/vm.hpp"

namespace discode
{

class VM_Pool
{

    std::map<std::string, std::shared_ptr<VM>> pool;
public:
    VM_Pool() {}

    VM * get(Socket * sock, std::string server_id) {
        if (!pool.count(server_id)) {
            pool.insert(std::pair<std::string, std::shared_ptr<VM>>(server_id, std::make_shared<VM>(sock, server_id)));
        }
        return pool.at(server_id).get();
    }

    // Todo: Better scheduling algorithm
    void stepall() {
        for (auto const& vm : pool) {
            vm.second->step();
        }
    }

};

};
#endif