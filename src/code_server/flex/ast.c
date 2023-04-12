

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

AST_Node * createNull(short lineno)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_NULL;
    node->lineno = lineno;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createBool(short lineno, AST_Bool val)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_BOOL;
    node->lineno = lineno;
    node->asBool = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createNumber(short lineno, char * value)
{
    char *_;
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_NUM;
    node->lineno = lineno;
    node->asDouble = strtod(value, &_);
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createString(short lineno, char * value)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_STR;
    node->lineno = lineno;
    node->asStr = malloc(strlen(value)+1);
    strcpy(node->asStr, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

AST_Node * createIdentifier(short lineno, char * value)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = AST_NODE_IDENT;
    node->lineno = lineno;
    node->asStr = malloc(strlen(value)+1);
    strcpy(node->asStr, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Utility functions
AST_Node * createUnaryExpr(short lineno, AST_NODE_TYPE operation, AST_Node * rhs)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = operation;
    node->lineno = lineno;
    node->left = NULL;
    node->right = rhs;
    return node;
}

AST_Node * createBinaryExpr(short lineno, AST_NODE_TYPE operation, AST_Node * lhs, AST_Node * rhs)
{
    AST_Node * node = malloc(sizeof(AST_Node));
    node->type = operation;
    node->lineno = lineno;
    node->left = lhs;
    node->right = rhs;
    return node;
}

// Unary expressions
AST_Node * createNegExpr(short lineno, AST_Node * lhs) { return createUnaryExpr(lineno, AST_NODE_NEG, lhs); }
AST_Node * createNotExpr(short lineno, AST_Node * lhs) { return createUnaryExpr(lineno, AST_NODE_NOT, lhs); }

// Binary expressions
AST_Node * createAddExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_ADD, lhs, rhs); }
AST_Node * createSubExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_SUB, lhs, rhs); }
AST_Node * createMulExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_MUL, lhs, rhs); }
AST_Node * createDivExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_DIV, lhs, rhs); }
AST_Node * createModExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_MOD, lhs, rhs); }
AST_Node * createAndExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_AND, lhs, rhs); }
AST_Node * createOrExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_OR, lhs, rhs); }
AST_Node * createEqExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_EQ, lhs, rhs); }
AST_Node * createGtrExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_GTR, lhs, rhs); }
AST_Node * createGteqExpr(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_GTEQ, lhs, rhs); }

// Name resolution
AST_Node * createIdentifierPath(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_RESOLVE_NAME, lhs, rhs); }
AST_Node * createGlobalResolution(short lineno, AST_Node * lhs) { return createUnaryExpr(lineno, AST_NODE_GLOBAL_SCOPE, lhs); }
AST_Node * createLibraryResolution(short lineno, AST_Node * lhs) { return createUnaryExpr(lineno, AST_NODE_LIB_SCOPE, lhs); }

// Lists
AST_Node * createIdentList(short lineno, AST_Node * rhs) { return createUnaryExpr(lineno, AST_NODE_IDENT_LIST_START, rhs); }
AST_Node * createIdentListElement(short lineno, AST_Node * lhs, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_RESOLVE_NAME, lhs, rhs); }

AST_Node * createExprList(short lineno, AST_Node * rhs) { return createUnaryExpr(lineno, AST_NODE_ARG_LIST_START, rhs); }
AST_Node * createExprListElement(short lineno, AST_Node * expr, AST_Node * rhs) { return createBinaryExpr(lineno, AST_NODE_ARG_LIST_CONT, expr, rhs); }

AST_Node * createStatementList(short lineno, AST_Node * current, AST_Node * next) { return createBinaryExpr(lineno, AST_NODE_STATEMENT_LIST, current, next); }

AST_Node * createAssign(short lineno, AST_Node * dest, AST_Node * expr) { return createBinaryExpr(lineno, AST_NODE_ASSIGN, dest, expr); }
AST_Node * createFCall(short lineno, AST_Node * src, AST_Node * args) { return createBinaryExpr(lineno, AST_NODE_FCALL, src, args); }
AST_Node * createMCall(short lineno, AST_Node * src, AST_Node * ident, AST_Node * args) { return createBinaryExpr(lineno, AST_NODE_MCALL, createIndex(lineno, src, ident), args); }
AST_Node * createReturn(short lineno, AST_Node * retValue) { return createUnaryExpr(lineno, AST_NODE_RETURN, retValue); }
AST_Node * createIf(short lineno, AST_Node * eval, AST_Node * statements) { return createBinaryExpr(lineno, AST_NODE_IF, eval, statements); }
AST_Node * createBranchingPath(short lineno, AST_Node * eval, AST_Node * statements) { return createBinaryExpr(lineno, AST_NODE_BRANCH, eval, statements); }
AST_Node * createConditionalEvaluator(short lineno, AST_Node * eval, AST_Node * statements) { return createBinaryExpr(lineno, AST_NODE_ALSO, eval, statements); }
AST_Node * createWhile(short lineno, AST_Node * eval, AST_Node * statements) { return createBinaryExpr(lineno, AST_NODE_WHILE, eval, statements); }
AST_Node * createFor(short lineno, AST_Node * eval, AST_Node * statements) { return createBinaryExpr(lineno, AST_NODE_FOR, eval, statements); }
AST_Node * createForArgs(short lineno, AST_Node * iterval, AST_Node * init, AST_Node * comp) { return createBinaryExpr(lineno, AST_NODE_FOR_ARGS, iterval, createBinaryExpr(lineno, AST_NODE_FOR_ARGS, init, comp)); }

AST_Node * createDeclare(short lineno, AST_Node * dest, AST_Node * expr) { return createBinaryExpr(lineno, AST_NODE_DECLARE, dest, expr); }
AST_Node * createDeclareFunction(short lineno, AST_Node * signature, AST_Node * code) { return createBinaryExpr(lineno, AST_NODE_DECLARE_FUNCTION, signature, code); }
AST_Node * createDeclareMethod(short lineno, AST_Node * signature, AST_Node * code) { return createBinaryExpr(lineno, AST_NODE_DECLARE_METHOD, signature, code); }

AST_Node * createSignature(short lineno, AST_Node * identifier, AST_Node * arglist) { return createBinaryExpr(lineno, AST_NODE_FUNCTION_SIGNATURE, identifier, arglist); }

AST_Node * createIndex(short lineno, AST_Node * base, AST_Node * index) { return createBinaryExpr(lineno, AST_NODE_INDEX, base, index); }

AST_Node * createArray(short lineno, AST_Node * elements) { return createUnaryExpr(lineno, AST_NODE_ARRAY_DEF, elements); }
AST_Node * createObject(short lineno, AST_Node * elements) { return createUnaryExpr(lineno, AST_NODE_OBJECT_DEF, elements); }
AST_Node * createKeyValueList(short lineno, AST_Node * keyvalue, AST_Node * next) { return createBinaryExpr(lineno, AST_NODE_OBJECT_LIST, keyvalue, next); }
AST_Node * createKeyValuePair(short lineno, AST_Node * key, AST_Node * value) { return createBinaryExpr(lineno, AST_NODE_KEY_VALUE_PAIR, key, value); }

AST_Node * createObjectDefinition(short lineno, AST_Node * def) { return createUnaryExpr(lineno, AST_NODE_ARRAY_DEF, def); }

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
        case AST_NODE_MOD:
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
        case AST_NODE_MCALL:
        case AST_NODE_IF:
        case AST_NODE_ALSO:
        case AST_NODE_BRANCH:
        case AST_NODE_WHILE:
        case AST_NODE_FOR:
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
        
        default:
            assert(25);
    }

    free(node);
}


