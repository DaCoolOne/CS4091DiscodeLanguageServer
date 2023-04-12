#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include "lang/vm.hpp"

extern "C"
{
    #include "flex/ast.h"

    AST_Node * parse(const char *, Parse_Error *);
    void freeAST(AST_Node *);
}

namespace discode {

void analyze_file(std::string path);
void analyze_string(std::string str);

void load(discode::VM * vm, AST_Node * node, std::string channel, std::string msg_id);
void loadVM(discode::VM * vm, std::string channel, std::string path, std::string msg_id);
void loadVM_string(discode::VM * vm, std::string channel, std::string str, std::string msg_id);

std::vector<std::string> getArgs(AST_Node * node);

};

namespace discode_internal {

std::vector<std::string> getArgs(AST_Node * node);
std::vector<std::shared_ptr<discode::Instruction>> buildConstant(AST_Node * constant);
std::vector<std::shared_ptr<discode::Instruction>> buildArray(AST_Node * expr);
std::vector<std::shared_ptr<discode::Instruction>> buildObject(AST_Node * expr);
std::vector<std::shared_ptr<discode::Instruction>> constructNameResolution(AST_Node * name, AST_NODE_TYPE scope);
std::vector<std::shared_ptr<discode::Instruction>> resolveName(AST_Node * name);
std::pair<std::vector<std::shared_ptr<discode::Instruction>>, uint16_t> buildArgList(AST_Node * arglist);
std::vector<std::shared_ptr<discode::Instruction>> buildFunctionCall(AST_Node * fcall);
std::vector<std::shared_ptr<discode::Instruction>> buildMethodCall(AST_Node * mcall);
std::vector<std::shared_ptr<discode::Instruction>> buildExpressionEval(AST_Node * expr);
std::vector<std::shared_ptr<discode::Instruction>> buildAssign(AST_Node * assign, uint16_t jumpoffset);
std::vector<std::shared_ptr<discode::Instruction>> buildStatement(AST_Node * statement, uint16_t jumpoffset);
std::vector<std::shared_ptr<discode::Instruction>> buildStatements(AST_Node * node, uint16_t jumpoffset = 0);
std::pair<std::shared_ptr<discode::Function>, std::string> buildFunction(AST_Node * node, bool isMethod = false);
std::vector<std::shared_ptr<discode::Instruction>> buildBinaryOperator(AST_Node * node);
std::vector<std::shared_ptr<discode::Instruction>> buildUnaryOperator(AST_Node * node);
std::vector<std::shared_ptr<discode::Instruction>> buildAssignReturn(AST_Node * ret_node);
std::vector<std::shared_ptr<discode::Instruction>> buildReturn(AST_Node * ret_node);
std::vector<std::shared_ptr<discode::Instruction>> buildIfStatement(AST_Node * if_statement, uint16_t jumpoffset);
std::vector<std::vector<std::shared_ptr<discode::Instruction>>> buildAlsoChain(AST_Node * also_root, uint16_t jumpoffset);
std::vector<std::shared_ptr<discode::Instruction>> buildWhileStatement(AST_Node * while_statement, uint16_t jumpoffset);
std::vector<std::shared_ptr<discode::Instruction>> buildForStatement(AST_Node * for_statement, uint16_t jumpoffset);
std::vector<std::shared_ptr<discode::Instruction>> buildIndexResolve(AST_Node * index);

};
#endif