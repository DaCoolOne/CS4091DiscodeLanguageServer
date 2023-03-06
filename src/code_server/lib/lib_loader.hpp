#ifndef LIB_LOADER
#define LIB_LOADER

#include "lang/data.hpp"
#include "lang/vm.hpp"
#include <vector>

namespace lib {

class LibFunction : public discode::Data
{
    std::vector<discode::Type> type_vec;
    std::string name;
public:
    LibFunction(std::vector<discode::Type> types): discode::Data(discode::Type::TYPE_INTERNAL_FUNCTION), type_vec(types) {}
    std::string repr() {
        std::string s = "FUNCTION(";
        for (uint16_t i = 0; i < type_vec.size(); i ++) {
            if(i) { s += ", "; }
            s += "arg" + std::to_string(i);
        }
        return s + ")";
    }
    void call(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data);
    virtual std::shared_ptr<discode::Data> execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data)
        { throw std::logic_error("Library function is not implemented"); }
};

class LibLoader
{
    std::string base;
    std::map<std::string, std::shared_ptr<LibFunction>> functions;
public:
    LibLoader(std::string libname): base(libname) { }

    void add(std::string name, std::shared_ptr<LibFunction> function) { functions.insert(std::pair<std::string, std::shared_ptr<LibFunction>>(name, function)); }

    void load(discode::VM * vm);
};

};
#endif