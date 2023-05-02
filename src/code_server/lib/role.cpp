#include "lib/lib_loader.hpp"
#include "lib/role.hpp"
#include "lang/vm.hpp"

#include <iostream>
#include "utils/json.hpp"

std::shared_ptr<discode::Data> lib_role::Send::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    
    json::JsonObject obj;
    obj.add("Name", std::make_shared<json::JsonString>("interaction"));
    obj.add("Server_ID", std::make_shared<json::JsonString>(data.at(0)->getString()));
    obj.add("Role", std::make_shared<json::JsonString>(data.at(1)->getString()));
    obj.add("User", std::make_shared<json::JsonString>(data.at(2)->getString()));
    vm->sendObject(&obj);

    return std::make_shared<discode::Null>();
}