

#include "lang/data.hpp"
#include "utils/string.hpp"

std::string discode::getTypeStr(discode::Type type) {
    switch(type) {
        case discode::Type::TYPE_ARRAY:     return "Array";
        case discode::Type::TYPE_BOOL:      return "Bool";
        case discode::Type::TYPE_FUNCTION:  return "Function";
        case discode::Type::TYPE_METHOD:    return "Method";
        case discode::Type::TYPE_NULL:      return "Null";
        case discode::Type::TYPE_NUMBER:    return "Number";
        case discode::Type::TYPE_OBJECT:    return "Object";
        case discode::Type::TYPE_STRING:    return "String";
        default:                            return "ERR_TYPE";
    }
}

std::string discode::getTypeStr(std::vector<discode::Type> types) {
    std::string out = "";
    for(uint8_t i = 0; i < types.size(); ++i)
    {
        discode::Type t = types[i];
        out += getTypeStr(t);
        if (i + 1 == types.size()) {
            continue;
        }
        if (types.size() > 2) {
            out += ", ";
            if (i + 2 == types.size()) {
                out += "or ";
            }
        }
        else {
            out += " or ";
        }
    }
    return out;
}
