

#include "lib/lib_loader.hpp"
#include "lang/data.hpp"
#include "lang/vm.hpp"
#include <vector>

void lib::LibFunction::call(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data)
{
    if (type_vec.size() != data.size()) {
        vm->error(discode::ErrorBadArgumentCount(type_vec.size()));
        return;
    }

    for (uint16_t i = 0; i < type_vec.size(); ++i)
    {
        if(type_vec.at(i) != data.at(i)->type) {
            vm->error(discode::ErrorUnexpectedType(type_vec.at(i), data.at(i)->type));
            return;
        }
    }

    vm->push(this->execute(vm, data));
}

void lib::LibLoader::load(discode::VM * vm) {
    for(auto const& f : functions) {
        vm->writeLib(base, f.first, f.second);
    }
}

