#ifndef ARRAY_HPP
#define ARRAY_HPP

#include "lib/lib_loader.hpp"

namespace lib_array
{

class Length : public lib::LibFunction
{
public:
    Length(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_ARRAY }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Append : public lib::LibFunction
{
public:
    Append(): lib::LibFunction(std::vector<std::vector<discode::Type>>{ std::vector<discode::Type>{
        discode::Type::TYPE_ARRAY
    }, std::vector<discode::Type>{
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

class Array : public LibLoader
{
public:
    Array(): LibLoader("array") {
        add("length", std::make_shared<lib_array::Length>());
    }
};

};
#endif