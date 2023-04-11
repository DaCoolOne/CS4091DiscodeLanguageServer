#ifndef CONV_HPP
#define CONV_HPP

#include "lib/lib_loader.hpp"

namespace lib_conv
{

class ToString : public lib::LibFunction
{
public:
    ToString(): lib::LibFunction(std::vector<std::vector<discode::Type>> { std::vector<discode::Type>{ discode::Type::TYPE_NUMBER, discode::Type::TYPE_BOOL, discode::Type::TYPE_STRING } }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class ToNumber : public lib::LibFunction
{
public:
    ToNumber(): lib::LibFunction(std::vector<std::vector<discode::Type>>{ std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_BOOL } }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class ToBool : public lib::LibFunction
{
public:
    ToBool(): lib::LibFunction(std::vector<std::vector<discode::Type>>{ std::vector<discode::Type>{
        discode::Type::TYPE_ARRAY,
        discode::Type::TYPE_BOOL,
        discode::Type::TYPE_FUNCTION,
        discode::Type::TYPE_INTERNAL_FUNCTION,
        discode::Type::TYPE_METHOD,
        discode::Type::TYPE_NULL,
        discode::Type::TYPE_NUMBER,
        discode::Type::TYPE_OBJECT,
        discode::Type::TYPE_STRING
    } }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Conv : public LibLoader
{
public:
    Conv(): LibLoader("conv") {
        add("str", std::make_shared<lib_conv::ToString>());
        add("num", std::make_shared<lib_conv::ToNumber>());
        add("bool", std::make_shared<lib_conv::ToBool>());
    }
};

};
#endif