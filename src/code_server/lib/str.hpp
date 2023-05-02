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

class Strip : public lib::LibFunction
{
public:
    Strip(): lib::LibFunction({ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Upper : public lib::LibFunction
{
public:
    Upper(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Lower : public lib::LibFunction
{
public:
    Lower(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class PopBack : public lib::LibFunction
{
public:
    PopBack(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Count : public lib::LibFunction
{
public:
    Count(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class FirstOf : public lib::LibFunction
{
public:
    FirstOf(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

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
        add("strip", std::make_shared<lib_str::Strip>());
        add("upper", std::make_shared<lib_str::Upper>());
        add("lower", std::make_shared<lib_str::Lower>());
        add("popback", std::make_shared<lib_str::PopBack>());
        add("count", std::make_shared<lib_str::Count>());
        add("firstof", std::make_shared<lib_str::FirstOf>());
    }
};

};
#endif