#ifndef AST_H
#define AST_H

#include <stdio.h>

typedef unsigned short uint16;
typedef char AST_Bool;
#define TRUE 1
#define FALSE 0

typedef enum {
    AST_NODE_NULL,
    AST_NODE_BOOL,
    AST_NODE_STR,
    AST_NODE_NUM,

    AST_NODE_ADD,
    AST_NODE_SUB,
    AST_NODE_MUL,
    AST_NODE_DIV,
    AST_NODE_AND,
    AST_NODE_OR,
    AST_NODE_NOT,
    AST_NODE_NEG,
    AST_NODE_EQ,
    AST_NODE_GTR,
    AST_NODE_GTEQ,

    AST_NODE_IDENT,
    AST_NODE_RESOLVE_NAME,
    AST_NODE_GLOBAL_SCOPE,
    AST_NODE_LIB_SCOPE,

    AST_NODE_IDENT_LIST_START,
    AST_NODE_IDENT_LIST_CONT,
    AST_NODE_ARG_LIST_START,
    AST_NODE_ARG_LIST_CONT,

    AST_NODE_STATEMENT_LIST,

    AST_NODE_ASSIGN,
    AST_NODE_FCALL,
    AST_NODE_RETURN,
    AST_NODE_IF,
    AST_NODE_WHILE,

    AST_NODE_DECLARE,
    AST_NODE_DECLARE_FUNCTION,
    AST_NODE_DECLARE_METHOD,

    AST_NODE_FUNCTION_SIGNATURE,

    AST_NODE_INDEX,
    AST_NODE_ARRAY_DEF,
    AST_NODE_OBJECT_DEF,
    AST_NODE_KEY_VALUE_PAIR,
    AST_NODE_OBJECT_LIST
} AST_NODE_TYPE;

struct AST_Node_raw
{
    AST_NODE_TYPE type;
    struct AST_Node_raw * left;
    struct AST_Node_raw * right;

    union data
    {
        AST_Bool asBool;
        char * asStr;
        double asDouble;
    };

};
typedef struct AST_Node_raw AST_Node;

// Specialized getters
AST_Bool getBool(AST_Node * node);
char * getStr(AST_Node * node);
double getDouble(AST_Node * node);

AST_Node * createNull();
AST_Node * createBool(AST_Bool val);
AST_Node * createNumber(char * value);
AST_Node * createString(char * value);
AST_Node * createIdentifier(char * value);

// Unary expressions
AST_Node * createNegExpr(AST_Node * lhs);
AST_Node * createNotExpr(AST_Node * lhs);

// Binary exptressions
AST_Node * createAddExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createSubExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createMulExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createDivExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createAndExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createOrExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createEqExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createGtrExpr(AST_Node * lhs, AST_Node * rhs);
AST_Node * createGteqExpr(AST_Node * lhs, AST_Node * rhs);

// Name resolution operations
AST_Node * createIdentifierPath(AST_Node * lhs, AST_Node * rhs);
AST_Node * createGlobalResolution(AST_Node * lhs);
AST_Node * createLibraryResolution(AST_Node * lhs);

// Lists
AST_Node * createIdentList(AST_Node * rhs);
AST_Node * createIdentListElement(AST_Node * value, AST_Node * rhs);

AST_Node * createExprList(AST_Node * rhs);
AST_Node * createExprListElement(AST_Node * expr, AST_Node * rhs);

AST_Node * createStatementList(AST_Node * current, AST_Node * next);

// Statement types
AST_Node * createAssign(AST_Node * dest, AST_Node * expr);
AST_Node * createFCall(AST_Node * src, AST_Node * args);
AST_Node * createReturn(AST_Node * retValue);
AST_Node * createIf(AST_Node * eval, AST_Node * statements);
AST_Node * createWhile(AST_Node * eval, AST_Node * statements);

// Outer level declarations
AST_Node * createDeclare(AST_Node * dest, AST_Node * expr);
AST_Node * createDeclareFunction(AST_Node * signature, AST_Node * code);
AST_Node * createDeclareMethod(AST_Node * signature, AST_Node * code);

AST_Node * createSignature(AST_Node * identifier, AST_Node * arglist);

// Indexing, arrays, objects
AST_Node * createIndex(AST_Node * base, AST_Node * index);

AST_Node * createArray(AST_Node * elements);
AST_Node * createObject(AST_Node * elements);
AST_Node * createKeyValueList(AST_Node * keyvalue, AST_Node * next);
AST_Node * createKeyValuePair(AST_Node * key, AST_Node * value);

// Memory management
void freeAST(AST_Node * node);

void set_yyin(FILE * new_yyin);
int feof_yyin();

#endif