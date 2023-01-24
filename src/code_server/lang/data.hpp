#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include <map>
#include <memory>

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

struct Data
{
    Type type;
    std::shared_ptr<void> data;

    Data(): type(Type::TYPE_NULL), data(nullptr) {}
    Data(bool * _data): type(Type::TYPE_BOOL), data(_data) {}
    Data(double * _data): type(Type::TYPE_NUMBER), data(_data) {}
    Data(std::string * _data): type(Type::TYPE_STRING), data(_data) {}
    Data(std::vector<Data> * _data): type(Type::TYPE_ARRAY), data(_data) {}
    Data(std::map<std::string, Data> * _data): type(Type::TYPE_OBJECT), data(_data) {}

    void swap(Data &other) { std::swap(type, other.type); data.swap(other.data); }
};

};
#endif