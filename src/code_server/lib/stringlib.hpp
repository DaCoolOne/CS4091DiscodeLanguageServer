#ifndef STRINGLIB_HPP
#define STRINGLIB_HPP

#include "lib/lib_loader.hpp"

namespace lib_stringlib
{

class Strip : public lib::LibFunction
{
public:
    Strip(): lib::LibFunction({ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Stringlib : public LibLoader
{
public:
    Stringlib(): LibLoader("stringlib") {
        add("strip", std::make_shared<lib_stringlib::Strip>());
    }
};

};
#endif