

#include "lib/array.hpp"

std::shared_ptr<discode::Data> lib_array::Length::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    return std::make_shared<discode::Number>(data.at(0)->getVector()->size());
}

std::shared_ptr<discode::Data> lib_array::Append::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    auto arr = data.at(0);
    arr->getVector()->push_back(data.at(1));
    return arr;
}
