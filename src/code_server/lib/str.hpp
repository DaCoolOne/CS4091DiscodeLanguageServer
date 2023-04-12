#ifndef STR_HPP
#define STR_HPP

#include "lib/lib_loader.hpp"

namespace lib_str
{

class Length : public lib::LibFunction
{
public:
    Length(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Substr : public lib::LibFunction
{
public:
    Substr(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_NUMBER }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Str : public LibLoader
{
public:
    Str(): LibLoader("str") {
        add("length", std::make_shared<lib_str::Length>());
        add("substr", std::make_shared<lib_str::Substr>());
    }
};

};
#endif