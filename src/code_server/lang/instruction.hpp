#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>

namespace discode {

enum InstructionType
{
    INS_NOP,
    INS_PUSH,
    INS_POP,
    INS_ADD,
    INS_SUB,
    INS_MUL,
    INS_DIV,
    INS_FCALL,
};

typedef std::vector<std::string> NamedReference;

};
#endif