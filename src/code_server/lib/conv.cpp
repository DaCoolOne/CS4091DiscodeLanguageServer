

#include "conv.hpp"
#include <string>

std::shared_ptr<discode::Data> lib_conv::ToString::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    std::shared_ptr<discode::Data> arg0 = data.at(0);
    if(arg0->type == discode::Type::TYPE_NUMBER) {
        std::string raw = std::to_string(arg0->getNumber());
        if(raw.find('.') != std::string::npos) {
            while(raw.back() == '0') {
                raw.pop_back();
            }
        }
        if(raw.back() == '.') {
            raw.pop_back();
        }
        return std::make_shared<discode::String>(raw);
    }
    else if(arg0->type == discode::Type::TYPE_BOOL) {
        return std::make_shared<discode::String>(arg0->getBool() ? "true" : "false");
    }
    else if(arg0->type == discode::Type::TYPE_STRING) {
        return std::make_shared<discode::String>(arg0->getString());
    }
    else {
        return std::make_shared<discode::String>(arg0->repr());
    }
}

std::shared_ptr<discode::Data> lib_conv::ToNumber::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    std::shared_ptr<discode::Data> arg0 = data.at(0);
    if(arg0->type == discode::Type::TYPE_STRING) {
        try {
            std::size_t idx;
            double val = std::stod(arg0->getString(), &idx); // May throw
            if(idx != arg0->getString().size()) {
                vm->error(discode::Error("Unabled to convert "+arg0->getString()+" to a number"));
                return std::make_shared<discode::Null>();
            }
            return std::make_shared<discode::Number>(val);
        }
        catch(...) {
            vm->error(discode::Error("Unabled to convert "+arg0->getString()+" to a number"));
            return std::make_shared<discode::Null>();
        }
    }
    else if(arg0->type == discode::Type::TYPE_BOOL) {
        return std::make_shared<discode::Number>(arg0->getBool() ? 1 : 0);
    }
    else if(arg0->type == discode::Type::TYPE_NUMBER) {
        return std::make_shared<discode::Number>(arg0->getNumber());
    }
    else {
        vm->error(discode::Error("Unabled to convert to number"));
        return std::make_shared<discode::Null>();
    }
}

std::shared_ptr<discode::Data> lib_conv::ToBool::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    std::shared_ptr<discode::Data> arg0 = data.at(0);
    if (arg0->type == discode::Type::TYPE_NULL || (arg0->type == discode::Type::TYPE_BOOL && !arg0->getBool())) {
        return std::make_shared<discode::Boolean>(false);
    }
    else {
        return std::make_shared<discode::Boolean>(true);
    }
}
