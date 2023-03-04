#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>

namespace json {

enum JsonDataType
{
    JSON_DATA_NULL,
    JSON_DATA_BOOL,
    JSON_DATA_NUMBER,
    JSON_DATA_STRING,
    JSON_DATA_ARRAY,
    JSON_DATA_OBJECT
};

class JsonData
{
private:
    JsonDataType _type;
public:
    JsonData(JsonDataType type): _type(type) {}
    virtual double asNumber() { throw std::logic_error("Could not decode number"); }
    virtual bool asBool() { throw std::logic_error("Could not decode bool"); }
    virtual std::string asString() { throw std::logic_error("Could not decode string"); }
    virtual std::vector<std::shared_ptr<JsonData>> asVector() { throw std::logic_error("Could not decode vector"); }
    virtual std::shared_ptr<JsonData> at(std::string key) { throw std::logic_error("Could not decode vector"); }

    virtual std::string toJsonString() { throw std::exception(); return ""; }

    JsonDataType type() { return _type; }
};

class JsonNull: public JsonData
{
public:
    JsonNull(): JsonData(JsonDataType::JSON_DATA_NULL) {}

    std::string toJsonString() override { return "null"; }
};

class JsonBool: public JsonData
{
    bool _data;
public:
    JsonBool(bool value): JsonData(JsonDataType::JSON_DATA_BOOL), _data(value) {}

    bool asBool() override { return _data; }

    std::string toJsonString() override { return _data ? "true" : "false"; }
};

class JsonNumber: public JsonData
{
    double _data;
public:
    JsonNumber(double value): JsonData(JsonDataType::JSON_DATA_NUMBER), _data(value) {}

    double asNumber() override { return _data; }

    std::string toJsonString() override;
};

class JsonString: public JsonData
{
    std::string _data;
public:
    JsonString(std::string value): JsonData(JsonDataType::JSON_DATA_STRING), _data(value) {}

    std::string asString() override { return _data; }

    std::string toJsonString() override;
};

class JsonArray: public JsonData
{
private:
    std::vector<std::shared_ptr<JsonData>> _data;

public:
    JsonArray(): JsonData(JsonDataType::JSON_DATA_ARRAY) {}

    std::vector<std::shared_ptr<JsonData>> asVector() override { return _data; }
    void add(std::shared_ptr<JsonData> value) { _data.push_back(value); }

    std::string toJsonString() override;
};

class JsonObject: public JsonData
{
private:
    std::map<std::string, std::shared_ptr<JsonData>> _data;

public:
    JsonObject(): JsonData(JsonDataType::JSON_DATA_OBJECT) {}

    std::shared_ptr<JsonData> at(std::string key) override { return _data.at(key); }
    void add(std::string key, std::shared_ptr<JsonData> value) { _data.insert(std::pair<std::string, std::shared_ptr<JsonData>>(key, value)); }

    std::string toJsonString() override;
};

std::shared_ptr<JsonData> loadJsonData(std::istream& in);

std::string getTypeName(JsonDataType type);

};

#endif