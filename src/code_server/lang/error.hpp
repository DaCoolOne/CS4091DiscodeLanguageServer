#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include <string>
#include <stdexcept>
#include "lang/scope.hpp"
#include "utils/string.hpp"

namespace discode
{

class Error
{
    std::string message;
    std::vector<discode::NamedReference> backtrace;
public:
    Error(std::string msg): message(msg) {}

    void pushBt(discode::NamedReference bt) { backtrace.push_back(bt); }

    std::string what() {
        std::vector<std::string> lines(backtrace.size() + 1);
        lines.push_back("Error " + message);
        for (int i = 0; i < backtrace.size(); i ++) {
            lines.push_back("\tin " + backtrace.at(i).stringRepr());
        }
        return util::join(lines, '\n');
    }
};

};
#endif