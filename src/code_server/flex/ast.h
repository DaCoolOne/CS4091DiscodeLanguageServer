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
    AST_NODE_MOD,
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
    AST_NODE_MCALL,
    AST_NODE_RETURN,
    AST_NODE_IF,
    AST_NODE_BRANCH,
    AST_NODE_ALSO,
    AST_NODE_WHILE,
    AST_NODE_FOR,
    AST_NODE_FOR_ARGS,

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
    unsigned short lineno;
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

#define MAX_ERR_LEN 256
struct Parse_Error_raw
{
    char txt[MAX_ERR_LEN];
    unsigned short line;
};
typedef struct Parse_Error_raw Parse_Error;

// Specialized getters
AST_Bool getBool(AST_Node * node);
char * getStr(AST_Node * node);
double getDouble(AST_Node * node);

AST_Node * createNull(short lineno);
AST_Node * createBool(short lineno, AST_Bool val);
AST_Node * createNumber(short lineno, char * value);
AST_Node * createString(short lineno, char * value);
AST_Node * createIdentifier(short lineno, char * value);

// Unary expressions
AST_Node * createNegExpr(short lineno, AST_Node * lhs);
AST_Node * createNotExpr(short lineno, AST_Node * lhs);

// Binary exptressions
AST_Node * createAddExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createSubExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createMulExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createDivExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createModExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createAndExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createOrExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createEqExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createGtrExpr(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createGteqExpr(short lineno, AST_Node * lhs, AST_Node * rhs);

// Name resolution operations
AST_Node * createIdentifierPath(short lineno, AST_Node * lhs, AST_Node * rhs);
AST_Node * createGlobalResolution(short lineno, AST_Node * lhs);
AST_Node * createLibraryResolution(short lineno, AST_Node * lhs);

// Lists
AST_Node * createIdentList(short lineno, AST_Node * rhs);
AST_Node * createIdentListElement(short lineno, AST_Node * value, AST_Node * rhs);

AST_Node * createExprList(short lineno, AST_Node * rhs);
AST_Node * createExprListElement(short lineno, AST_Node * expr, AST_Node * rhs);

AST_Node * createStatementList(short lineno, AST_Node * current, AST_Node * next);

// Statement types
AST_Node * createAssign(short lineno, AST_Node * dest, AST_Node * expr);
AST_Node * createFCall(short lineno, AST_Node * src, AST_Node * args);
AST_Node * createMCall(short lineno, AST_Node * src, AST_Node * ident, AST_Node * args);
AST_Node * createReturn(short lineno, AST_Node * retValue);
AST_Node * createIf(short lineno, AST_Node * eval, AST_Node * statements);
AST_Node * createBranchingPath(short lineno, AST_Node * path1, AST_Node * path2);
AST_Node * createConditionalEvaluator(short lineno, AST_Node * eval, AST_Node * statements);
AST_Node * createWhile(short lineno, AST_Node * eval, AST_Node * statements);
AST_Node * createFor(short lineno, AST_Node * eval, AST_Node * statements);
AST_Node * createForArgs(short lineno, AST_Node * iterval, AST_Node * init, AST_Node * comp);

// Outer level declarations
AST_Node * createDeclare(short lineno, AST_Node * dest, AST_Node * expr);
AST_Node * createDeclareFunction(short lineno, AST_Node * signature, AST_Node * code);
AST_Node * createDeclareMethod(short lineno, AST_Node * signature, AST_Node * code);

AST_Node * createSignature(short lineno, AST_Node * identifier, AST_Node * arglist);

// Indexing, arrays, objects
AST_Node * createIndex(short lineno, AST_Node * base, AST_Node * index);

AST_Node * createArray(short lineno, AST_Node * elements);
AST_Node * createObject(short lineno, AST_Node * elements);
AST_Node * createKeyValueList(short lineno, AST_Node * keyvalue, AST_Node * next);
AST_Node * createKeyValuePair(short lineno, AST_Node * key, AST_Node * value);

AST_Node * createObjectDefinition(short lineno, AST_Node * def);

// Memory management
void freeAST(AST_Node * node);

void set_yyin(FILE * new_yyin);
int feof_yyin();

#endif