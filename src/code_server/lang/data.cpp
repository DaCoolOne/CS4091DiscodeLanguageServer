

#include "lang/data.hpp"

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