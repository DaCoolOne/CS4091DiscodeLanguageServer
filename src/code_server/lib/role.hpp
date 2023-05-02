#ifndef ROLE_HPP
#define ROLE_HPP

#include "lib/lib_loader.hpp"

namespace lib_role
{

class Send : public lib::LibFunction
{
public:
    Send(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Role : public LibLoader
{
public:
    Role(): LibLoader("role") {
        add("send", std::make_shared<lib_role::Send>());
    }
};

};
#endif