#ifndef INTERACTION_HPP
#define INTERACTION_HPP

#include "lib/lib_loader.hpp"

namespace lib_interaction
{

class Send : public lib::LibFunction
{
public:
    Send(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_STRING, discode::Type::TYPE_STRING }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

};

namespace lib
{

class Interaction : public LibLoader
{
public:
    Interaction(): LibLoader("interaction") {
        add("send", std::make_shared<lib_interaction::Send>());
    }
};

};
#endif