

#include "conv.hpp"
#include <string>

std::shared_ptr<discode::Data> lib_conv::NumToString::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    std::string raw = std::to_string(data.at(0)->getNumber());
    if(raw.find('.') != std::string::npos) {
        while(raw.back() == '0') {
            raw.pop_back();
        }
    }
    if(raw.back() == '.') {
        raw.pop_back();
    }
    return std::make_shared<discode::String>(raw);
}

