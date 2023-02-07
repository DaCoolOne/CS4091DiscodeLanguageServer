#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

extern "C"
{
    #include "flex/ast.h"

    AST_Node * parse(const char *);
    void freeAST(AST_Node *);
}

namespace discode {

void analyze_file(std::string path);
void analyze_string(std::string string);

};
#endif