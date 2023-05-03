#ifndef MATH_HPP
#define MATH_HPP

#include "lib/lib_loader.hpp"

namespace lib_math
{

class Sum: public lib::LibFunction
{
public:
    Sum(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_ARRAY }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Mean: public lib::LibFunction
{
public:
    Mean(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_ARRAY }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Random: public lib::LibFunction
{
public:
    Random(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_NUMBER, discode::Type::TYPE_NUMBER }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Max:  public lib::LibFunction
{
public:
    Max(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_ARRAY }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Min:   public lib::LibFunction
{
public:
    Min(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_ARRAY }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Power:   public lib::LibFunction
{
public:
    Power(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_NUMBER, discode::Type::TYPE_NUMBER }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Sqrt:   public lib::LibFunction
{
public:
    Sqrt(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_NUMBER }) {}
    
    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};

class Mod:   public lib::LibFunction
{
public:
    Mod(): lib::LibFunction(std::vector<discode::Type>{ discode::Type::TYPE_NUMBER, discode::Type::TYPE_NUMBER }) {}

    std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) override;
};


};

namespace lib
{

class Math : public LibLoader
{
public:
    Math(): LibLoader("math") {
        add("sum", std::make_shared<lib_math::Sum>());
        add("mean", std::make_shared<lib_math::Mean>());
        add("max", std::make_shared<lib_math::Max>());
        add("min", std::make_shared<lib_math::Min>());
        add("random", std::make_shared<lib_math::Random>());
        add("power", std::make_shared<lib_math::Power>());
        add("sqrt", std::make_shared<lib_math::Sqrt>());
        add("mod", std::make_shared<lib_math::Mod>());
    }
};

};
#endif