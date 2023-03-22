

#include <stdlib.h>
#include <string.h>

#include "flex/ast.h"

AST_Bool getBool(AST_Node * node)
{
    return node->asBool;
}
char * getStr(AST_Node * node)
{
    return node->asStr;
}
double getDouble(AST_Node * node)
{
    return node->asDouble;
}

AST_Node * createNull()
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createBool(AST_Bool val)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_BOOL;
    node->asBool = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createNumber(char * value)
{
    char *_;
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_NUM;
    node->asDouble = strtod(value, &_);
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createString(char * value)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_STR;
    node->asStr = malloc(strlen(value)+1);
    strcpy(node->asStr, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createIdentifier(char * value)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_IDENT;
    node->asStr = malloc(strlen(value)+1);
    strcpy(node->asStr, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Utility functions
AST_Node * createUnaryExpr(AST_NODE_TYPE operation, AST_Node * rhs)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = operation;
    node->left = NULL;
    node->right = rhs;
    return node;
}

AST_Node * createBinaryExpr(AST_NODE_TYPE operation, AST_Node * lhs, AST_Node * rhs)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = operation;
    node->left = lhs;
    node->right = rhs;
    return node;
}

// Unary expressions
AST_Node * createNegExpr(AST_Node * lhs) { return createUnaryExpr(AST_NODE_NEG, lhs); }
AST_Node * createNotExpr(AST_Node * lhs) { return createUnaryExpr(AST_NODE_NOT, lhs); }

// Binary expressions
AST_Node * createAddExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_ADD, lhs, rhs); }
AST_Node * createSubExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_SUB, lhs, rhs); }
AST_Node * createMulExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_MUL, lhs, rhs); }
AST_Node * createDivExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_DIV, lhs, rhs); }
AST_Node * createAndExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_AND, lhs, rhs); }
AST_Node * createOrExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_OR, lhs, rhs); }
AST_Node * createEqExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_EQ, lhs, rhs); }
AST_Node * createGtrExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_GTR, lhs, rhs); }
AST_Node * createGteqExpr(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_GTEQ, lhs, rhs); }

// Name resolution
AST_Node * createIdentifierPath(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_RESOLVE_NAME, lhs, rhs); }
AST_Node * createGlobalResolution(AST_Node * lhs) { return createUnaryExpr(AST_NODE_GLOBAL_SCOPE, lhs); }
AST_Node * createLibraryResolution(AST_Node * lhs) { return createUnaryExpr(AST_NODE_LIB_SCOPE, lhs); }

// Lists
AST_Node * createIdentList(AST_Node * rhs) { return createUnaryExpr(AST_NODE_IDENT_LIST_START, rhs); }
AST_Node * createIdentListElement(AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(AST_NODE_RESOLVE_NAME, lhs, rhs); }

AST_Node * createExprList(AST_Node * rhs) { return createUnaryExpr(AST_NODE_ARG_LIST_START, rhs); }
AST_Node * createExprListElement(AST_Node * expr, AST_Node * rhs) { return createBinaryExpr(AST_NODE_ARG_LIST_CONT, expr, rhs); }

AST_Node * createStatementList(AST_Node * current, AST_Node * next) { return createBinaryExpr(AST_NODE_STATEMENT_LIST, current, next); }

AST_Node * createAssign(AST_Node * dest, AST_Node * expr) { return createBinaryExpr(AST_NODE_ASSIGN, dest, expr); }
AST_Node * createFCall(AST_Node * src, AST_Node * args) { return createBinaryExpr(AST_NODE_FCALL, src, args); }
AST_Node * createReturn(AST_Node * retValue) { return createUnaryExpr(AST_NODE_RETURN, retValue); }
AST_Node * createIf(AST_Node * eval, AST_Node * statements) { return createBinaryExpr(AST_NODE_IF, eval, statements); }
AST_Node * createWhile(AST_Node * eval, AST_Node * statements) { return createBinaryExpr(AST_NODE_WHILE, eval, statements); }

AST_Node * createDeclare(AST_Node * dest, AST_Node * expr) { return createBinaryExpr(AST_NODE_DECLARE, dest, expr); }
AST_Node * createDeclareFunction(AST_Node * signature, AST_Node * code) { return createBinaryExpr(AST_NODE_DECLARE_FUNCTION, signature, code); }
AST_Node * createDeclareMethod(AST_Node * signature, AST_Node * code) { return createBinaryExpr(AST_NODE_DECLARE_METHOD, signature, code); }

AST_Node * createSignature(AST_Node * identifier, AST_Node * arglist) { return createBinaryExpr(AST_NODE_FUNCTION_SIGNATURE, identifier, arglist); }

AST_Node * createIndex(AST_Node * base, AST_Node * index) { return createBinaryExpr(AST_NODE_INDEX, base, index); }

AST_Node * createArray(AST_Node * elements) { return createUnaryExpr(AST_NODE_ARRAY_DEF, elements); }
AST_Node * createObject(AST_Node * elements) { return createUnaryExpr(AST_NODE_OBJECT_DEF, elements); }
AST_Node * createKeyValueList(AST_Node * keyvalue, AST_Node * next) { return createBinaryExpr(AST_NODE_OBJECT_LIST, keyvalue, next); }
AST_Node * createKeyValuePair(AST_Node * key, AST_Node * value) { return createBinaryExpr(AST_NODE_KEY_VALUE_PAIR, key, value); }

AST_Node * createObjectDefinition(AST_Node * def) { return createUnaryExpr(AST_NODE_ARRAY_DEF, def); }

void freeAST(AST_Node * node)
{
    switch (node->type)
    {
        // String constants
        case AST_NODE_IDENT:
        case AST_NODE_STR:
            free(node->asStr);
            break;
        
        // Unary operators
        case AST_NODE_NEG:
        case AST_NODE_NOT:
            if (node->right) freeAST(node->right);
            break;

        // Binary operators
        case AST_NODE_ADD:
        case AST_NODE_SUB:
        case AST_NODE_MUL:
        case AST_NODE_DIV:
        case AST_NODE_AND:
        case AST_NODE_OR:
        case AST_NODE_EQ:
        case AST_NODE_GTR:
        case AST_NODE_GTEQ:
            if (node->left) freeAST(node->left);
            if (node->right) freeAST(node->right);
            break;

        // Unary control flow
        case AST_NODE_GLOBAL_SCOPE:
        case AST_NODE_LIB_SCOPE:
        case AST_NODE_IDENT_LIST_START:
        case AST_NODE_ARG_LIST_START:
        case AST_NODE_RETURN:
            if (node->right) freeAST(node->right);
            break;

        // Binary control flow
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
        case AST_NODE_INDEX:
            if (node->left) freeAST(node->left);
            if (node->right) freeAST(node->right);
            break;
        
        // Special cases
        case AST_NODE_IDENT_LIST_CONT:
            if (node->right) freeAST(node->right);
            break;
    }

    free(node);
}


