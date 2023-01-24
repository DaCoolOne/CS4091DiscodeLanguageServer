

#include "utils/string.hpp"

std::vector<std::string> util::split(std::string str, char delim)
{
    std::vector<std::string> results;

    size_t startIdx = 0, endIdx = std::string::npos;

    while((endIdx = str.find(delim, startIdx)) != std::string::npos)
    {
        results.push_back(str.substr(startIdx, endIdx - startIdx));
        startIdx = endIdx + 1;
    }

    results.push_back(str.substr(startIdx));

    return results;
}

std::string util::join(std::vector<std::string> strs, char between) {
    std::string s;
    size_t total_len = 0;
    for(size_t i = 0; i < strs.size(); i ++) { total_len += strs.at(i).length(); }
    s.reserve(total_len + strs.size() * (between ? 1 : 0) + 1);
    for(size_t i = 0; i < strs.size(); i ++) {
        if (i && between) { s += between; }
        s += strs.at(i);
    }
    return s;
}

