

#include "lib/str.hpp"

std::shared_ptr<discode::Data> lib_str::Length::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    return std::make_shared<discode::Number>(data.at(0)->getString().size());
}

std::shared_ptr<discode::Data> lib_str::Substr::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto str = data.at(0)->getString();
    auto left_idx = static_cast<int32_t>(data.at(1)->getNumber() + 0.5);
    auto right_idx = static_cast<int32_t>(data.at(1)->getNumber() + 0.5);
    auto len = left_idx - right_idx;

    if (left_idx < 0) {
        vm->error(discode::ErrorOutOfBounds(left_idx));        
        return std::make_shared<discode::Null>();
    }
    if (len < 0 || left_idx + len > str.length()) {
        vm->error(discode::ErrorOutOfBounds(right_idx));        
        return std::make_shared<discode::Null>();
    }

    return std::make_shared<discode::String>(str.substr(left_idx, len));
}

