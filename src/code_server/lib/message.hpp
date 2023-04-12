#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "lib/lib_loader.hpp"

namespace lib_message
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

class Message : public LibLoader
{
public:
    Message(): LibLoader("message") {
        add("send", std::make_shared<lib_message::Send>());
    }
};

};
#endif