#include "stringlib.hpp"
#include "utils/string.hpp"
#include <string>

std::shared_ptr<discode::Data> lib_stringlib::Strip::execute(discode::VM * vm, std::vector<std::shared_ptr<discode::Data>> data) {
    std::string orig = data.at(0)->getString();
    std::string delims = data.at(1)->getString();
    std::string result = util::strip(orig, delims);
    return std::make_shared<discode::String>(result);
}