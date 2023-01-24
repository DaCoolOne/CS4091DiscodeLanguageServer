

#include "json.hpp"

// Local procedures
char nextNonWhitespace(std::istream & in)
{
    char c;
    do {
        if(!in.get(c)) { return EOF; }
    } while(c == ' ' || c == '\n' || c == '\t' || c == '\r');
    return c;
}

std::string outString(std::string in) {
    std::string res = "\"";
    for (int i = 0; i < in.length(); i ++) {
        switch (in.at(i)) {
            case '\n': res += "\\n";  break;
            case '\r': res += "\\r";  break;
            case '\t': res += "\\t";  break;
            case '\\': res += "\\\\"; break;
            case '\"': res += "\\\""; break;

            default: res += in.at(i);
        }
    }
    res += '"';
    return res;
}

std::string outNumber(double in) {
    return std::to_string(in);
}

std::string loadstring(std::istream & in)
{
    // Load a string
    std::string s;
    char c;
    bool escape = false, success = true;
    if(!in.get(c)) success = false;
    while(success && (escape || c != '"')) {
        if (c == '\n') throw std::logic_error("Unclosed string");
        if (escape) {
            switch (c)
            {
            case 'n' : s += '\n'; break;
            case 'r' : s += '\r'; break;
            case 't' : s += '\t'; break;
            case '\\': s += '\\'; break;
            case '"' : s += '"' ; break;
            case '\'': s += '\''; break;
            
            default:
                throw std::logic_error(std::string("Unknown escape sequence '\\") + c + '\'');
            }
            escape = false;
        }
        else {
            if (c == '\\') {
                escape = true;
            }
            else {
                s += c;
            }
        }
        if(!in.get(c)) success = false;
    }
    if (!success) throw std::logic_error("Unclosed string");
    return s;
}

bool readExpected(std::istream & in, std::string target)
{
    char c;
    for(int i = 0; i < target.length(); i ++) {
        if (!in.get(c)) return false;
        if (target[i] != c) return false;
    }
    return true;
}

std::shared_ptr<json::JsonData> _loadJsonDataRec(std::istream & in)
{

    // Look at first character in the stream to determine the json object type
    char c = nextNonWhitespace(in);
    if (c == EOF) {
        throw std::logic_error("Unexpected EOF");
    }

    std::shared_ptr<json::JsonData> data;

    if (c == '"') {
        // Load a string
        std::string s = loadstring(in);
        data = std::shared_ptr<json::JsonData>(new json::JsonString(s));
    }
    else if (c >= '0' && c <= '9') {
        // Load a number
        
    }
    else if (c == '{') {
        // Load an object
        c = nextNonWhitespace(in);
        json::JsonObject * jsonObject = new json::JsonObject();
        data = std::shared_ptr<json::JsonData>(jsonObject);
        while(c == '"') {
            std::string key = loadstring(in);
            c = nextNonWhitespace(in);
            if (c != ':') { throw std::logic_error("Missing colon"); }
            std::shared_ptr<json::JsonData> value = _loadJsonDataRec(in);
            jsonObject->add(key, value);
            c = nextNonWhitespace(in);
            if (c != ',') break;
            c = nextNonWhitespace(in);
        }
        if (c != '}') { throw std::logic_error("Malformed object"); }
    }
    else if (c == '[') {
        // Load an array
        c = nextNonWhitespace(in);
        json::JsonArray * jsonArray = new json::JsonArray();
        data = std::shared_ptr<json::JsonData>(jsonArray);
        while (c != ']') {
            std::shared_ptr<json::JsonData> value = _loadJsonDataRec(in);
            c = nextNonWhitespace(in);
            if (c != ',' && c != ']') throw std::logic_error("Malformed array");
            if (c == ',') c = nextNonWhitespace(in);
        }
    }
    else if (c == 't' || c == 'f') {
        // Load boolean
        if(readExpected(in, c == 't' ? "rue" : "alse")) {
            data = std::shared_ptr<json::JsonData>(new json::JsonBool(c == 't'));
        }
        else {
            throw std::logic_error("Unexpected character");
        }
    }
    else if (c == 'n') {
        // Load null
        if(readExpected(in, "ull")) {
            data = std::shared_ptr<json::JsonData>(new json::JsonNull());
        }
        else {
            throw std::logic_error("Unexpected character");
        }
    }
    else {
        throw std::logic_error("Unknown character");
    }

    return data;
}

std::shared_ptr<json::JsonData> json::loadJsonData(std::istream& in)
{

    std::shared_ptr<json::JsonData> data = _loadJsonDataRec(in);

    char c = nextNonWhitespace(in);
    if (c != EOF) {
        throw std::logic_error("Unexpected character, expected EOF");
    }

    return data;
}

std::string json::getTypeName(json::JsonDataType type)
{
    switch(type) {
        case json::JsonDataType::JSON_DATA_NULL:   return "Null";
        case json::JsonDataType::JSON_DATA_BOOL:   return "Boolean";
        case json::JsonDataType::JSON_DATA_NUMBER: return "Number";
        case json::JsonDataType::JSON_DATA_STRING: return "String";
        case json::JsonDataType::JSON_DATA_ARRAY:  return "Array";
        case json::JsonDataType::JSON_DATA_OBJECT: return "Object";

        default: return "Unknown";
    }
}

std::string json::JsonNumber::toJsonString()
{
    return outNumber(_data);
}

std::string json::JsonString::toJsonString()
{
    return outString(_data);
}

std::string json::JsonArray::toJsonString()
{
    std::string res = "[";
    for(int i = 0; i < _data.size(); i ++) {
        if (i != 0) {
            res += ',';
        }
        res += _data.at(i)->toJsonString();
    }
    res += ']';
    return res;
}

std::string json::JsonObject::toJsonString()
{
    std::string res = "{";
    for(std::map<std::string, std::shared_ptr<json::JsonData>>::iterator iter = _data.begin(); iter != _data.end(); iter++) {
        if (iter != _data.begin()) {
            res += ',';
        }
        res += outString(iter->first);
        res += ':';
        res += iter->second->toJsonString();
    }
    res += '}';
    return res;
}


