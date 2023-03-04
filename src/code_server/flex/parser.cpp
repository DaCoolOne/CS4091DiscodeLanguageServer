

#include "flex/parser.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "lang/function.hpp"

std::string getNodeName(AST_Node * node)
{
    switch (node->type)
    {
        case AST_NODE_ADD: return "ADD";
        case AST_NODE_SUB: return "SUB";
        case AST_NODE_MUL: return "MUL";
        case AST_NODE_DIV: return "DIV";
        case AST_NODE_AND: return "AND";
        case AST_NODE_OR: return "OR";
        case AST_NODE_EQ: return "EQ";
        case AST_NODE_GTR: return "GTR";
        case AST_NODE_GTEQ: return "GTEQ";
        case AST_NODE_RESOLVE_NAME: return "NAME_RESOLVE";
        case AST_NODE_ARG_LIST_CONT: return "ARG_LIST_CONT";
        case AST_NODE_STATEMENT_LIST: return "STATEMENT_LIST";
        case AST_NODE_ASSIGN: return "ASSIGN";
        case AST_NODE_FCALL: return "FCALL";
        case AST_NODE_IF: return "IF";
        case AST_NODE_WHILE: return "WHILE";
        case AST_NODE_DECLARE: return "DECLARE";
        case AST_NODE_DECLARE_FUNCTION: return "DECLARE_FUNCTION";
        case AST_NODE_DECLARE_METHOD: return "DECLARE_METHOD";
        case AST_NODE_FUNCTION_SIGNATURE: return "FUNCTION_SIG";
        case AST_NODE_NEG: return "NEG";
        case AST_NODE_NOT: return "NOT";
        case AST_NODE_GLOBAL_SCOPE: return "GLOBAL";
        case AST_NODE_LIB_SCOPE: return "LIBRARY";
        case AST_NODE_IDENT_LIST_START: return "NODE_IDENT_LIST_START";
        case AST_NODE_ARG_LIST_START: return "NODE_ARG_LIST_START";
        case AST_NODE_RETURN: return "RETURN";
        case AST_NODE_IDENT_LIST_CONT: return "IDENT_LIST_CONT";
    }

    return "Unknown";
}

void printNode(AST_Node * node, std::string indent)
{
    if (node == NULL) { return; }

    switch(node->type) {
        // Binary
        case AST_NODE_ADD:
        case AST_NODE_SUB:
        case AST_NODE_MUL:
        case AST_NODE_DIV:
        case AST_NODE_AND:
        case AST_NODE_OR:
        case AST_NODE_EQ:
        case AST_NODE_GTR:
        case AST_NODE_GTEQ:

        case AST_NODE_RESOLVE_NAME:
        case AST_NODE_ARG_LIST_CONT:
        case AST_NODE_STATEMENT_LIST:
        case AST_NODE_ASSIGN:
        case AST_NODE_FCALL:
        case AST_NODE_IF:
        case AST_NODE_WHILE:
        case AST_NODE_DECLARE:
        case AST_NODE_DECLARE_FUNCTION:
        case AST_NODE_DECLARE_METHOD:
        case AST_NODE_FUNCTION_SIGNATURE:

        case AST_NODE_IDENT_LIST_CONT:

            std::cout << indent << getNodeName(node) << std::endl;
            printNode(node->left, indent + "  ");
            printNode(node->right, indent + "  ");
        break;

        // Unary ops
        case AST_NODE_NEG:
        case AST_NODE_NOT:

        case AST_NODE_GLOBAL_SCOPE:
        case AST_NODE_LIB_SCOPE:
        case AST_NODE_IDENT_LIST_START:
        case AST_NODE_ARG_LIST_START:
        case AST_NODE_RETURN:
            std::cout << indent << getNodeName(node) << std::endl;
            printNode(node->right, indent + "  ");
        break;

        // Terminals
        case AST_NODE_NULL:
            std::cout << indent << "NULL" << std::endl;
        break;
        case AST_NODE_BOOL:
            std::cout << indent << (getBool(node) ? "Bool: TRUE" : "Bool: FALSE") << std::endl;
        break;
        case AST_NODE_NUM:
            std::cout << indent << "Number: " << getDouble(node) << std::endl;
        break;
        case AST_NODE_STR:
            std::cout << indent << "String: " << getStr(node) << std::endl;
        break;
        case AST_NODE_IDENT:
            std::cout << indent << "Ident: " << getStr(node) << std::endl;
        break;
    }
}

// Pre: AST_Node is of type AST_NODE_RESOLVE_NAME
std::vector<std::string> discode_internal::getArgs(AST_Node * node) {
    if(node == NULL) {
        return std::vector<std::string>();
    }
    std::vector<std::string> other = getArgs(node->right);
    std::vector<std::string> args;
    args.reserve(other.size() + 1);
    args.push_back(getStr(node->left));
    args.insert(args.end(), other.begin(), other.end());
    return args;
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::constructNameResolution(AST_Node * name, AST_NODE_TYPE scope) {
    if (name->left == nullptr) {
        std::vector<std::shared_ptr<discode::Instruction>> ins;
        
        if(scope == AST_NODE_LIB_SCOPE) {
            ins.push_back(std::make_shared<discode::InstructionGetLib>(getStr(name->right)));
        }
        else if(scope == AST_NODE_GLOBAL_SCOPE) {
            ins.push_back(std::make_shared<discode::InstructionGetGlobal>(getStr(name->right)));
        }
        else {
            ins.push_back(std::make_shared<discode::InstructionGetLocal>(getStr(name->right)));
        }
        return ins;
    }
    else if (name->type == AST_NODE_RESOLVE_NAME) {
        std::vector<std::shared_ptr<discode::Instruction>> resolution = discode_internal::constructNameResolution(name->left, scope);
        resolution.push_back(std::make_shared<discode::InstructionGetStack>(getStr(name->right)));
        return resolution;
    }
    else {
        throw std::logic_error("Invalid type during name resolution " + std::to_string(name->type));
    }
}

// Pre: node is of type AST_NODE_LIB_SCOPE, AST_NODE_GLOBAL_SCOPE, or AST_NODE_RESOLVE_NAME 
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::resolveName(AST_Node * name) {
    switch (name->type)
    {
        case AST_NODE_GLOBAL_SCOPE:
        case AST_NODE_LIB_SCOPE:
            return discode_internal::constructNameResolution(name->right, name->type);
        case AST_NODE_RESOLVE_NAME:
            return discode_internal::constructNameResolution(name, name->type);
    
        default:
            throw std::logic_error("Unknown NAME_RES node type " + std::to_string(name->type));
    }
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildConstant(AST_Node * constant) {
    std::shared_ptr<discode::Data> data;
    std::vector<std::shared_ptr<discode::Instruction>> ins_list;
    switch (constant->type)
    {
        case AST_NODE_NULL:
            data = std::make_shared<discode::Null>();
        break;
        case AST_NODE_BOOL:
            data = std::make_shared<discode::Boolean>(getBool(constant));
        break;
        case AST_NODE_NUM:
            data = std::make_shared<discode::Number>(getDouble(constant));
        break;
        case AST_NODE_STR:
            data = std::make_shared<discode::String>(getStr(constant));
        break;
    
        default:
            throw std::logic_error("Cannot create object of type " + std::to_string(constant->type));
        break;
    }
    if (ins_list.size() == 0 && data != nullptr) {
        ins_list.push_back(std::make_shared<discode::InstructionPush>(data));
    }
    return ins_list;
}

std::pair<std::vector<std::shared_ptr<discode::Instruction>>, uint16_t> discode_internal::buildArgList(AST_Node * arglist) {
    if (arglist == NULL) {
        return std::pair<std::vector<std::shared_ptr<discode::Instruction>>, uint16_t>(std::vector<std::shared_ptr<discode::Instruction>>(), 0);
    }
    
    auto rhs = discode_internal::buildArgList(arglist->right);

    auto lhs = discode_internal::buildExpressionEval(arglist->left);

    rhs.first.reserve(rhs.first.size() + lhs.size());
    rhs.first.insert(rhs.first.end(), lhs.begin(), lhs.end());

    ++rhs.second;

    return rhs;
}

// Pre: fcall is a AST_NODE_FCALL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildFunctionCall(AST_Node * fcall) {
    // Get the function arguments
    auto arglist = discode_internal::buildArgList(fcall->right->right);

    // Get the namespace of the function
    auto name_resolution = discode_internal::resolveName(fcall->left);

    arglist.first.reserve(name_resolution.size() + arglist.first.size() + 1);
    arglist.first.insert(arglist.first.end(), name_resolution.begin(), name_resolution.end());
    arglist.first.push_back(std::make_shared<discode::InstructionFunctionCall>(arglist.second));

    return arglist.first;
}

// Pre: expr is not NULL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildExpressionEval(AST_Node * expr) {
    switch (expr->type)
    {
        case AST_NODE_FCALL:
            return discode_internal::buildFunctionCall(expr);
        case AST_NODE_RESOLVE_NAME:
        case AST_NODE_LIB_SCOPE:
        case AST_NODE_GLOBAL_SCOPE:
            return discode_internal::resolveName(expr);
        
        case AST_NODE_NULL:
        case AST_NODE_BOOL:
        case AST_NODE_NUM:
        case AST_NODE_STR:
            return discode_internal::buildConstant(expr);
        
        default:
            throw std::logic_error("Unknown EXPR node type " + std::to_string(expr->type));
        break;
    }

    std::cout << "WARN: EXPR not implemented! " << expr->type << std::endl;
    return std::vector<std::shared_ptr<discode::Instruction>>();
}

std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildAssign(AST_Node * assign, uint16_t jumpoffset) {

    AST_Node * returns_to = assign->left;
    AST_Node * expr = assign->right;

    // Ignore return value
    if (returns_to == nullptr) {
        return discode_internal::buildExpressionEval(expr);
    }
    // Assign return value
    else {

        // Todo: Return value assignment
        return discode_internal::buildExpressionEval(expr);
    }

}

// Pre: statement is not NULL
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildStatement(AST_Node * statement, uint16_t jumpoffset) {
    switch (statement->type)
    {
        case AST_NODE_ASSIGN:
            return discode_internal::buildAssign(statement, jumpoffset);
        
        case AST_NODE_IF:
            
        break;
        case AST_NODE_WHILE:

        break;
        case AST_NODE_RETURN:
            
        break;

        default:
            throw std::logic_error("Unknown STATEMENT node type " + std::to_string(statement->type));
        break;
    }

    std::cout << "WARN: STATEMENT not implemented! " << statement->type << std::endl;
    return std::vector<std::shared_ptr<discode::Instruction>>();
}

// Pre: AST_Node is of type STATEMENT_LIST
std::vector<std::shared_ptr<discode::Instruction>> discode_internal::buildStatements(AST_Node * node, uint16_t jumpoffset) {
    if (node == NULL) {
        return std::vector<std::shared_ptr<discode::Instruction>>();
    }
    auto ins1 = discode_internal::buildStatement(node->left, jumpoffset);

    if (node->right != NULL) {
        if (node->right->type != AST_NODE_STATEMENT_LIST) { throw std::logic_error("Expected statement list! 2"); }
        auto ins2 = discode_internal::buildStatements(node->right, jumpoffset + ins1.size());
        if (ins2.size()) {
            ins1.reserve(ins1.size() + ins2.size());
            ins1.insert(ins1.end(), ins2.begin(), ins2.end());
        }
    }

    return ins1;
}

// Pre: AST_Node is of type function
std::pair<std::shared_ptr<discode::Function>, std::string> discode_internal::buildFunction(AST_Node * node) {
    AST_Node * f_sig = node->left;
    if (f_sig->type != AST_NODE_FUNCTION_SIGNATURE) { throw std::logic_error("Expected function signature!"); }

    AST_Node * f_name = f_sig->left;
    if (f_name->type != AST_NODE_IDENT) { throw std::logic_error("Expected function name!"); }
    std::string function_name = getStr(f_name);

    std::vector<std::string> arglist = discode_internal::getArgs(f_sig->right->right);

    AST_Node * f_body = node->right;
    if (f_body->type != AST_NODE_STATEMENT_LIST) { throw std::logic_error("Expected statement list!"); }

    std::vector<std::shared_ptr<discode::Instruction>> instructions = discode_internal::buildStatements(f_body);

    return std::pair<std::shared_ptr<discode::Function>, std::string>(std::make_shared<discode::Function>(arglist, instructions), function_name);
}

// Loads an AST into the VM
void discode::load(discode::VM * vm, AST_Node * node, std::string channel)
{
    std::pair<std::shared_ptr<discode::Function>, std::string> func;
    switch (node->type)
    {
        case AST_NODE_DECLARE:
            // Todo
        break;
        case AST_NODE_DECLARE_FUNCTION:
            func = discode_internal::buildFunction(node);
            vm->writeGlobal(channel, func.second, func.first);
        break;
        case AST_NODE_DECLARE_METHOD:
            // Todo
        break;
        default:
            throw std::logic_error("Unknown entry node type");
        break;
    }
}

void discode::analyze_file(std::string path)
{
    AST_Node * ast = parse(path.c_str());

    // Print the tree
    printNode(ast, "");

    // Print the generated code
    if (ast->type == AST_NODE_DECLARE_FUNCTION) {
        auto func = discode_internal::buildFunction(ast);
        std::cout << func.second << " = " << func.first->repr() << std::endl;
    }
    else {
        std::cout << "Unknown node" << std::endl;
    }

    freeAST(ast);
}

void discode::analyze_string(std::string str)
{
    std::cout << str << std::endl;

    std::ofstream out;
    out.open("temp.txt");
    out << str;
    out.close();

    discode::analyze_file("temp.txt");
}

void discode::loadVM(discode::VM * vm, std::string channel, std::string path)
{
    AST_Node * ast = parse(path.c_str());

    load(vm, ast, channel);

    freeAST(ast);
}

void discode::loadVM_string(discode::VM * vm, std::string channel, std::string str)
{
    std::ofstream out;
    out.open("temp.txt");
    out << str;
    out.close();

    discode::loadVM(vm, channel, "temp.txt");
}
