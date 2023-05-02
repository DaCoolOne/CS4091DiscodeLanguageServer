

#include "lib/libs.hpp"

#include "lib/message.hpp"
#include "lib/interaction.hpp"
#include "lib/conv.hpp"
#include "lib/str.hpp"
#include "lib/array.hpp"

void lib::loadAll(discode::VM * vm) {

    lib::Conv().load(vm);
    lib::Message().load(vm);
    lib::Interaction().load(vm);
    lib::Str().load(vm);
    lib::Array().load(vm);

}

