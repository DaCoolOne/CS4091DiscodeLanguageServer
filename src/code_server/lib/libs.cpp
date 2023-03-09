

#include "lib/libs.hpp"

#include "lib/message.hpp"
#include "lib/conv.hpp"

void lib::loadAll(discode::VM * vm) {

    lib::Message().load(vm);
    lib::Conv().load(vm);

}

