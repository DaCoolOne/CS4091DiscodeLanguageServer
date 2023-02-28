#ifndef OBJECT_H
#define OBJECT_H

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
    TYPE_METHOD
};

enum OPCODE
{
    ADD,
    SUB,
    DIV,
    MUL,
    ICONST,
    HALT
};

struct Data
{
    Type type;

    Data(Type _type): type(_type) {}

    virtual bool getBool() { throw std::logic_error("Could not convert object to bool"); }
    virtual double getNumber() { throw std::logic_error("Could not convert object to double"); }
    virtual std::string getString() { throw std::logic_error("Could not convert object to string"); }
    virtual std::vector<std::shared_ptr<Data>> * getVector() { throw std::logic_error("Could not convert object to vector"); }
    virtual std::map<std::string, std::shared_ptr<Data>> * getMap() { throw std::logic_error("Could not convert object to map"); }
};

class Null : Data
{
public:
    Null(): Data(Type::TYPE_NULL) {}
};

class Boolean : Data
{
    bool val;
public:
    Boolean(bool value): Data(Type::TYPE_STRING), val(value) {}
    bool getBool() { return val; }
};

class Number : Data
{
    double val;
public:
    Number(double value): Data(Type::TYPE_STRING), val(value) {}
    double getNumber() { return val; }
};

struct String : Data
{
    std::string val;
public:
    String(std::string &value): Data(Type::TYPE_STRING), val(value) {}
    std::string getString() { return val; }
};

struct Array : Data
{
    std::vector<std::shared_ptr<Data>> data;
public:
    Array(std::vector<std::shared_ptr<Data>> &value): Data(Type::TYPE_STRING), data(value) {}
    std::vector<std::shared_ptr<Data>> * getVector() { return &data; }
};

struct Object : Data
{
    std::map<std::string, std::shared_ptr<Data>> data;
public:
    Object(std::map<std::string, std::shared_ptr<Data>> &value): Data(Type::TYPE_STRING), data(value) {}
    std::map<std::string, std::shared_ptr<Data>> * getMap() { return &data; }
};

};
#endif