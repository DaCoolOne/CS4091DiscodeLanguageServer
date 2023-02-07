

#include "flex/parser.hpp"

#include <iostream>
#include <fstream>

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
        case AST_NODE_IDENT_LIST_START: return "NODE_ARG";
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

void discode::analyze_file(std::string path)
{
    AST_Node * ast = parse(path.c_str());

    // Print the tree
    printNode(ast, "");

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
