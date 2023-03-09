#ifndef CONV_HPP
#define CONV_HPP

#include "lib/lib_loader.hpp"

namespace lib_conv
{

class NumToString : public lib::LibFunction
{
public:
    NumToString(): lib::LibFunction({ discode::Type::TYPE_NUMBER }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Conv : public LibLoader
{
public:
    Conv(): LibLoader("conv") {
        add("str", std::make_shared<lib_conv::NumToString>());
    }
};

};
#endif