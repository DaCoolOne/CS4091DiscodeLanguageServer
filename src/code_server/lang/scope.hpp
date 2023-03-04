#ifndef SCOPE_HPP
#define SCOPE_HPP

#include <map>
#include <stdexcept>
#include "lang/data.hpp"
#include "utils/string.hpp"

namespace discode
{

typedef std::map<std::string, std::shared_ptr<Data>> Scope;

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
        return "UNKNOWN";
    }

    bool resolveOrMake(Scope * scope, Data ** out) {
        // Todo

        return false;
    }
    bool resolve(Scope * scope, Data ** out) {
        // Todo

        return false;
    }
};


};
#endif