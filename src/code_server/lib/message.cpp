

#include "lib/lib_loader.hpp"
#include "lib/message.hpp"
#include "lang/vm.hpp"

#include <iostream>
#include "utils/json.hpp"

std::shared_ptr<discode::Data> lib_message::Send::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    
    json::JsonObject obj;
    obj.add("Name", std::make_shared<json::JsonString>("Send Message"));
    obj.add("Channel_id", std::make_shared<json::JsonString>(data.at(0)->getString()));
    obj.add("Message", std::make_shared<json::JsonString>(data.at(1)->getString()));
    vm->sendObject(&obj);

    return std::make_shared<discode::Null>();
}

