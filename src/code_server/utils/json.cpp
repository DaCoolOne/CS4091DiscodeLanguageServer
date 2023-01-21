

#include "json.hpp"

// Local procedures
char nextNonWhitespace(std::istream & in) {
    char c;
    do {
        in.get(c);
    } while(c == ' ' || c == '\n' || c == '\t' || c == '\r');
    return c;
}

std::string loadstring(std::istream & in) {
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
                throw std::logic_error("Unknown escape sequence");
            }
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

bool readExpected(std::istream & in, std::string target) {
    char c;
    for(int i = 0; i < target.length(); i ++) {
        if (!in.get(c)) return false;
        if (target[i] != c) return false;
    }
    return true;
}

std::shared_ptr<json::JsonData> _loadJsonDataRec(std::istream & in) {

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
        do {
            std::shared_ptr<json::JsonData> value = _loadJsonDataRec(in);
            c = nextNonWhitespace(in);
        } while (c == ',');
        if(c != ']') { throw std::logic_error("Malformed array"); }
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

std::shared_ptr<json::JsonData> json::loadJsonData(std::istream& in) {
    
    std::shared_ptr<json::JsonData> data = _loadJsonDataRec(in);

    if (!in.eof()) {
        throw std::logic_error("Unexpected EOF");
    }

    return data;
}
