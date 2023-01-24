#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <map>
#include "lang/data.hpp"
#include "utils/string.hpp"

namespace discode
{

typedef std::map<std::string, Data> Scope;

enum ScopeType
{
    SCOPE_LOCAL,
    SCOPE_GLOBAL,
    SCOPE_LIBRARY,
};

class NamedReference
{
    std::vector<std::string> _path;
    ScopeType _scopetype;

public:
    NamedReference(std::string rep) {
        // util::split(rep, '.');
    }
    NamedReference(ScopeType scopeType, std::string base): _scopetype(scopeType) { _path.push_back(base); }

    ScopeType scopeType() { return _scopetype; }
    void appendRef(std::string ref) { _path.push_back(ref); }

    std::string stringRepr() {
        switch (_scopetype)
        {
            case ScopeType::SCOPE_LOCAL: return util::join(_path);
            case ScopeType::SCOPE_GLOBAL: return "#" + util::join(_path);
            case ScopeType::SCOPE_LIBRARY: return "!" + util::join(_path);
        }
    }

    bool resolveOrMake(Scope * scope, Data &out) {
        std::string key;
        size_t max_elmnt = _path.size() - 1;
        out.type = Type::TYPE_OBJECT;
        
        for (int i = 0; i < max_elmnt; i ++) {
            scope = static_cast<std::map<std::string, Data>*>(out.data.get());
            key = _path.at(i);
            if (!scope->count(key)) {
                return false;
            }
            out = scope->at(_path.at(i));
            if (out.type != Type::TYPE_OBJECT) {
                return false;
            }
        }

        scope = static_cast<std::map<std::string, Data>*>(out.data.get());
        key = _path.at(max_elmnt);
        if (!scope->count(key)) {
            out = Data();
            scope->insert(std::pair<std::string, Data>(key, out));
            return false;
        }
        out = scope->at(_path.at(max_elmnt));

        return true;
    }
    bool resolve(Scope * scope, Data &out) {
        std::string key;
        out.type = Type::TYPE_OBJECT;
        for (int i = 0; i < _path.size(); i ++) {
            if (out.type != Type::TYPE_OBJECT) {
                return false;
            }
            scope = static_cast<std::map<std::string, Data>*>(out.data.get());
            key = _path.at(i);
            if (!scope->count(key)) {
                return false;
            }
            out = scope->at(_path.at(i));
        }
        return true;
    }
};


};
#endif