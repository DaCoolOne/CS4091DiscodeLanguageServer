#ifndef STRING_HPP
#define STRING_HPP

#include <vector>
#include <string>

namespace util {

std::vector<std::string> split(std::string str, char delim = '\n');

std::string join(std::vector<std::string> strs, char between = '\0');

std::string unescape(std::string orig);

};
#endif