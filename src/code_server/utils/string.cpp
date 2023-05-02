

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

std::string util::strip(std::string str, std::string chars) {
  std::string s = str;
  size_t start_pos = 0;
  while (start_pos < str.length() &&
         chars.find(str[start_pos]) != std::string::npos) { start_pos++; }
  s.erase(0, start_pos);

  size_t end_pos = s.length() - 1;
  while (end_pos > 0 && chars.find(s[end_pos]) != std::string::npos) { end_pos--; }
  s.erase(end_pos + 1);

  return s;
}

std::string util::unescape(std::string orig) {
    std::string out;
    out.reserve(orig.size());
    bool escaped = false;
    for(char c : orig) {
        if(escaped) {
            switch (c)
            {
                case 'n': out += '\n'; break;
                case 't': out += '\t'; break;
                case 'r': out += '\r'; break;
                
                default: out += c;
            }
            escaped = false;
        }
        else if(c == '\\') {
            escaped = true;
        }
        else {
            out += c;
        }
    }
    return out;
}

