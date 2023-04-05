

#include "lib/libs.hpp"

#include "lib/message.hpp"
#include "lib/conv.hpp"
#include "lib/stringlib.hpp"

void lib::loadAll(discode::VM * vm) {

    lib::Message().load(vm);
    lib::Conv().load(vm);
    lib::Stringlib().load(vm);

}

