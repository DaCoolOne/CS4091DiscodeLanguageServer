

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

std::shared_ptr<discode::Data> lib_str::Strip::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    auto delims = data.at(1)->getString();
    auto result = util::strip(orig, delims);
    return std::make_shared<discode::String>(result);
}

std::shared_ptr<discode::Data> lib_str::Upper::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    for (size_t i = 0; i < orig.length(); i++) { orig[i] = std::toupper(orig[i]); }
    return std::make_shared<discode::String>(orig);
}

std::shared_ptr<discode::Data> lib_str::Lower::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    for (size_t i = 0; i < orig.length(); i++) { orig[i] = std::tolower(orig[i]); }
    return std::make_shared<discode::String>(orig);
}

std::shared_ptr<discode::Data> lib_str::PopBack::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    if (orig.empty()) { return std::make_shared<discode::Null>(); }
    orig.pop_back();
    return std::make_shared<discode::String>(orig);
}

std::shared_ptr<discode::Data> lib_str::Count::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    auto key = data.at(1)->getString();
    int counter = 0;
    for (size_t i = 0; i < orig.length(); i++) {
        if (orig[i] == key[0]) { counter++; }
    }
    return std::make_shared<discode::Number>(counter);
}

std::shared_ptr<discode::Data> lib_str::FirstOf::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto orig = data.at(0)->getString();
    auto key = data.at(1)->getString();
    auto keyLen = key.length();
    for (size_t i = 0; i < orig.length(); i++) {
        if (orig[i] == key[0] && i + keyLen <= orig.length()) {
            size_t inc = 0;
            while (i + inc < orig.length() && orig[i+inc] == key[inc]) { 
                inc++;
                if (inc == keyLen) { return std::make_shared<discode::Number>(i); }
            }
        }
    }
    auto result = key + " not found";
    return std::make_shared<discode::String>(result);
}
