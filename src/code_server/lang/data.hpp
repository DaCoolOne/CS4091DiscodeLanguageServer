#ifndef DATA_HPP
#define DATA_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

namespace discode {

enum Type
{
    TYPE_NULL,
    TYPE_BOOL,
    TYPE_NUMBER,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_OBJECT,
    TYPE_FUNCTION,
    TYPE_METHOD,
    TYPE_INTERNAL_FUNCTION
};

struct Data
{
    Type type;

    Data(Type _type): type(_type) {}

    virtual std::string repr() { throw std::logic_error("String repr not defined"); }
    virtual std::string deepRepr(std::string indent = "") { return repr(); }

    virtual std::shared_ptr<Data> copy() { throw std::logic_error("Copy not defined for this object"); }

    virtual bool getBool() { throw std::logic_error("Could not convert object to bool"); }
    virtual double getNumber() { throw std::logic_error("Could not convert object to double"); }
    virtual std::string getString() { throw std::logic_error("Could not convert object to string"); }
    virtual std::vector<std::shared_ptr<Data>> * getVector() { throw std::logic_error("Could not convert object to vector"); }
    virtual std::map<std::string, std::shared_ptr<Data>> * getMap() { throw std::logic_error("Could not convert object to map"); }
};

class Null : public Data
{
public:
    Null(): Data(Type::TYPE_NULL) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<Null>(); }
    std::string repr() override { return "NULL"; }
};

class Boolean : public Data
{
    bool val;
public:
    Boolean(bool value): Data(Type::TYPE_BOOL), val(value) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<Boolean>(val); }
    bool getBool() override { return val; }
    std::string repr() override { return val ? "TRUE" : "FALSE"; }
};

class Number : public Data
{
    double val;
public:
    Number(double value): Data(Type::TYPE_NUMBER), val(value) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<Number>(val); }
    double getNumber() override { return val; }
    std::string repr() override { return std::to_string(val); }
};

class String : public Data
{
    std::string val;
public:
    String(const char * value): Data(Type::TYPE_STRING), val(value) {}
    String(const std::string &value): Data(Type::TYPE_STRING), val(value) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<String>(val); }
    std::string getString() override { return val; }
    std::string repr() override { return val; }
};

class Array : public Data
{
    std::vector<std::shared_ptr<Data>> data;
public:
    Array(): Data(Type::TYPE_ARRAY) {}
    Array(std::vector<std::shared_ptr<Data>> &value): Data(Type::TYPE_ARRAY), data(value) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<Array>(data); }
    std::vector<std::shared_ptr<Data>> * getVector() override { return &data; }
    std::string repr() override { return "ARRAY[" + std::to_string(data.size()) + "]"; }
    std::string deepRepr(std::string indent) override {
        std::string out = "[\n";
        for(auto const& it : data)
        {
            out += indent + "  " + it->deepRepr(indent + "  ") + "\n";
        }
        return out + indent + "]";
    }
};

class Object : public Data
{
    std::map<std::string, std::shared_ptr<Data>> data;
public:
    Object(): Data(Type::TYPE_OBJECT) {}
    Object(std::map<std::string, std::shared_ptr<Data>> &value): Data(Type::TYPE_OBJECT), data(value) {}
    std::shared_ptr<Data> copy() override { return std::make_shared<Object>(data); }
    std::map<std::string, std::shared_ptr<Data>> * getMap() override { return &data; }
    std::string repr() override { return "Object { ... }"; }
    std::string deepRepr(std::string indent) override {
        std::string out = "{\n";
        for(auto const& it : data)
        {
            out += indent + "  " + it.first + ": " + it.second->deepRepr(indent + "  ") + "\n";
        }
        return out + indent + "}";
    }
};

std::string getTypeStr(Type type);
std::string getTypeStr(std::vector<Type> types);

};
#endif