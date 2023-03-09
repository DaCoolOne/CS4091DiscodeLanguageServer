

#include "conv.hpp"
#include <string>

std::shared_ptr<discode::Data> lib_conv::NumToString::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    return std::make_shared<discode::String>(std::to_string(data.at(0)->getNumber()));
}

