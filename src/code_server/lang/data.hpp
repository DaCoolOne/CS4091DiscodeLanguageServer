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

    Data(): type(TYPE_NULL), data(nullptr) {}
    Data(bool * data): type(TYPE_BOOL), data(data) {}
    Data(double * data): type(TYPE_NUMBER), data(data) {}
    Data(std::string * data): type(TYPE_STRING), data(data) {}
    Data(std::vector<Data> * data): type(TYPE_ARRAY), data(data) {}
    Data(std::map<std::string, Data> * data): type(TYPE_ARRAY), data(data) {}
};

class Function
{

};

};
#endif