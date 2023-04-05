
%code top {

#include <stdio.h>
#include <string.h>
#include "flex/ast.h"

void yyerror(char *s);
int yylex();

#define YYERROR_VERBOSE

AST_Node * parsedStatement;
Parse_Error * parseError;

extern int yylineno;

}

%union {

AST_Node* node;

}

%token KEYWORD_IF
%token KEYWORD_WHILE
%token KEYWORD_RETURN
%token KEYWORD_DECLARE
%token KEYWORD_FUNCTION
%token KEYWORD_METHOD
%token KEYWORD_NULL
%token KEYWORD_TRUE
%token KEYWORD_FALSE
%token KEYWORD_AND
%token KEYWORD_OR
%token KEYWORD_NOT

%token <node> NUMBER
%token <node> STRING

%token <node> IDENTIFIER

%token OPERATOR_EQ
%token OPERATOR_NEQ
%token OPERATOR_LSEQ
%token OPERATOR_GTEQ

%token OPERATOR_ASSIGN

%token OPERATOR_ADD
%token OPERATOR_SUB
%token OPERATOR_MUL
%token OPERATOR_DIV
%token OPERATOR_LSS
%token OPERATOR_GTR
%token OPERATOR_MOD

%token OPERATOR_DOT
%token OPERATOR_COMMA
%token OPERATOR_POUND
%token OPERATOR_EXCLAM
%token OPERATOR_COLON
%token OPERATOR_SEMICOLON
%token OPERATOR_OPEN_BRACE
%token OPERATOR_CLOSE_BRACE
%token OPERATOR_OPEN_SQUARE
%token OPERATOR_CLOSE_SQUARE
%token OPERATOR_OPEN_PAREN
%token OPERATOR_CLOSE_PAREN

%start START

%%

START               : FUNCTION  { parsedStatement = $<node>1; }
                    | METHOD    { parsedStatement = $<node>1; }
                    | DECLARE   { parsedStatement = $<node>1; }
                    ;

FUNCTION            : KEYWORD_FUNCTION IDENTIFIER OPERATOR_COLON IDENTIFIER_LIST CODE_BLOCK     { $<node>$ = createDeclareFunction(yylineno, createSignature(yylineno, $<node>2, $<node>4), $<node>5); }
                    ;

METHOD              : KEYWORD_METHOD IDENTIFIER OPERATOR_COLON IDENTIFIER_LIST CODE_BLOCK   { $<node>$ = createDeclareMethod(yylineno, createSignature(yylineno, $<node>2, $<node>4), $<node>5); }
                    ;

DECLARE             : KEYWORD_DECLARE FULL_IDENTIFIER OPERATOR_ASSIGN EXPRESSION OPERATOR_SEMICOLON     { $<node>$ = createDeclare(yylineno, $<node>2, $<node>4); }
                    ;

CODE_BLOCK          : OPERATOR_OPEN_BRACE STATEMENT_LIST OPERATOR_CLOSE_BRACE   { $<node>$ = $<node>2; }
                    ;

STATEMENT_LIST      :                           { $<node>$ = NULL; }
                    | STATEMENT STATEMENT_LIST  { $<node>$ = createStatementList(yylineno, $<node>1, $<node>2); }
                    ;

STATEMENT           : FULL_IDENTIFIER OPERATOR_ASSIGN EXPRESSION OPERATOR_SEMICOLON     { $<node>$ = createAssign(yylineno, $<node>1, $<node>3); }
                    | EXPRESSION OPERATOR_SEMICOLON                                     { $<node>$ = createAssign(yylineno, NULL, $<node>1); }
                    | KEYWORD_IF EXPRESSION CODE_BLOCK                                  { $<node>$ = createIf(yylineno, $<node>2, $<node>3); }
                    | KEYWORD_WHILE EXPRESSION CODE_BLOCK                               { $<node>$ = createWhile(yylineno, $<node>2, $<node>3); }
                    | KEYWORD_RETURN OPERATOR_SEMICOLON                                 { $<node>$ = createReturn(yylineno, NULL); }
                    | KEYWORD_RETURN EXPRESSION OPERATOR_SEMICOLON                      { $<node>$ = createReturn(yylineno, $<node>2); }
                    ;

FUNCTION_CALL       : TERMINAL OPERATOR_OPEN_PAREN ARGUMENT_LIST OPERATOR_CLOSE_PAREN    { $<node>$ = createFCall(yylineno, $<node>1, $<node>3); }
                    ;

ARGUMENT_LIST       :               { $<node>$ = createExprList(yylineno, NULL); }
                    | ARGUMENT_REP  { $<node>$ = createExprList(yylineno, $<node>1); }
                    ;

ARGUMENT_REP        : EXPRESSION                                { $<node>$ = createExprListElement(yylineno, $<node>1, NULL); }       
                    | EXPRESSION OPERATOR_COMMA ARGUMENT_REP    { $<node>$ = createExprListElement(yylineno, $<node>1, $<node>3); }   
                    ;

IDENTIFIER_LIST     :                   { $<node>$ = createIdentList(yylineno, NULL); }
                    | IDENTIFIER_REP    { $<node>$ = createIdentList(yylineno, $<node>1); }
                    ;

IDENTIFIER_REP      : IDENTIFIER                                { $<node>$ = createIdentListElement(yylineno, $<node>1, NULL); }                       
                    | IDENTIFIER OPERATOR_COMMA IDENTIFIER_REP  { $<node>$ = createIdentListElement(yylineno, $<node>1, $<node>3); }   
                    ;

FULL_IDENTIFIER     : OPERATOR_POUND IDENTIFIER_PATH    { $<node>$ = createGlobalResolution(yylineno, $<node>2); }
                    | OPERATOR_EXCLAM IDENTIFIER_PATH   { $<node>$ = createLibraryResolution(yylineno, $<node>2); }
                    | IDENTIFIER_PATH                   { $<node>$ = $<node>1; }
                    ;

IDENTIFIER_PATH     : IDENTIFIER                                { $<node>$ = createIdentifierPath(yylineno, NULL, $<node>1); }
                    | IDENTIFIER_PATH OPERATOR_DOT IDENTIFIER   { $<node>$ = createIdentifierPath(yylineno, $<node>1, $<node>3); }
                    ;

EXPRESSION          : UNARY_BOOL_EXPR
                    ;

INDEX               : TERMINAL OPERATOR_OPEN_SQUARE EXPRESSION OPERATOR_CLOSE_SQUARE     { $<node>$ = createIndex(yylineno, $<node>1, $<node>3); }
                    ;

ARRAY               : OPERATOR_OPEN_SQUARE ARGUMENT_LIST OPERATOR_CLOSE_SQUARE  { $<node>$ = createArray(yylineno, $<node>2); }
                    ;

OBJECT              : OPERATOR_OPEN_BRACE KEY_VALUE_LIST OPERATOR_CLOSE_BRACE   { $<node>$ = createObjectDefinition(yylineno, $<node>2); }
                    ;

KEY_VALUE_LIST      :                                                                       { $<node>$ = NULL; }
                    | IDENTIFIER OPERATOR_COLON EXPRESSION                                  { $<node>$ = createKeyValueList(yylineno, createKeyValuePair(yylineno, $<node>1, $<node>3), NULL); }
                    | IDENTIFIER OPERATOR_COLON EXPRESSION OPERATOR_COMMA KEY_VALUE_LIST    { $<node>$ = createKeyValueList(yylineno, createKeyValuePair(yylineno, $<node>1, $<node>3), $<node>5); }
                    ;

UNARY_BOOL_EXPR     : KEYWORD_NOT OR_EXPR   { $<node>$ = createNotExpr(yylineno, $<node>2); }
                    | OR_EXPR               { $<node>$ = $<node>1; }
                    ;

OR_EXPR             : OR_EXPR KEYWORD_OR AND_EXPR   { $<node>$ = createOrExpr(yylineno, $<node>1, $<node>3); }
                    | AND_EXPR                      { $<node>$ = $<node>1; }
                    ;

AND_EXPR            : AND_EXPR KEYWORD_AND COMP_EXPR    { $<node>$ = createAndExpr(yylineno, $<node>1, $<node>3); }
                    | COMP_EXPR                         { $<node>$ = $<node>1; }
                    ;

COMP_EXPR           : COMP_EXPR OPERATOR_LSS ADD_EXPR   { $<node>$ = createGtrExpr(yylineno, $<node>3, $<node>1); }
                    | COMP_EXPR OPERATOR_LSEQ ADD_EXPR  { $<node>$ = createGteqExpr(yylineno, $<node>3, $<node>1); }
                    | COMP_EXPR OPERATOR_GTR ADD_EXPR   { $<node>$ = createGtrExpr(yylineno, $<node>1, $<node>3); }
                    | COMP_EXPR OPERATOR_GTEQ ADD_EXPR  { $<node>$ = createGteqExpr(yylineno, $<node>1, $<node>3); }
                    | COMP_EXPR OPERATOR_NEQ ADD_EXPR   { $<node>$ = createNotExpr(yylineno, createEqExpr(yylineno, $<node>1, $<node>3)); }
                    | COMP_EXPR OPERATOR_EQ ADD_EXPR    { $<node>$ = createEqExpr(yylineno, $<node>1, $<node>3); }
                    | ADD_EXPR                          { $<node>$ = $<node>1; }
                    ;

ADD_EXPR            : ADD_EXPR OPERATOR_ADD MUL_EXPR    { $<node>$ = createAddExpr(yylineno, $<node>1, $<node>3); }
                    | ADD_EXPR OPERATOR_SUB MUL_EXPR    { $<node>$ = createSubExpr(yylineno, $<node>1, $<node>3); }
                    | MUL_EXPR                          { $<node>$ = $<node>1; }
                    ;

MUL_EXPR            : MUL_EXPR OPERATOR_MUL UNARY_MINUS     { $<node>$ = createMulExpr(yylineno, $<node>1, $<node>3); }
                    | MUL_EXPR OPERATOR_DIV UNARY_MINUS     { $<node>$ = createDivExpr(yylineno, $<node>1, $<node>3); }
                    | MUL_EXPR OPERATOR_MOD UNARY_MINUS     { $<node>$ = createModExpr(yylineno, $<node>1, $<node>3); }
                    | UNARY_MINUS                           { $<node>$ = $<node>1; }
                    ;

UNARY_MINUS         : OPERATOR_SUB TERMINAL     { $<node>$ = createNegExpr(yylineno, $<node>2); }
                    | TERMINAL                  { $<node>$ = $<node>1; }
                    ;

TERMINAL            : NUMBER                                                { $<node>$ = $<node>1; }
                    | STRING                                                { $<node>$ = $<node>1; }
                    | KEYWORD_FALSE                                         { $<node>$ = createBool(yylineno, FALSE); }
                    | KEYWORD_TRUE                                          { $<node>$ = createBool(yylineno, TRUE); }
                    | KEYWORD_NULL                                          { $<node>$ = createNull(yylineno); }
                    | FULL_IDENTIFIER                                       { $<node>$ = $<node>1; }
                    | OPERATOR_OPEN_PAREN EXPRESSION OPERATOR_CLOSE_PAREN   { $<node>$ = $<node>2; }
                    | FUNCTION_CALL                                         { $<node>$ = $<node>1; }
                    | INDEX                                                 { $<node>$ = $<node>1; }
                    | ARRAY                                                 { $<node>$ = $<node>1; }
                    | OBJECT                                                { $<node>$ = $<node>1; }
                    ;

%%

void yyerror(char *s) {
    strncpy(parseError->txt, s, MAX_ERR_LEN);
    parseError->line = yylineno;
}

AST_Node * parse(const char * fname, Parse_Error * err) {
    set_yyin(fopen(fname, "r"));

    parseError = err;
    parsedStatement = NULL;
    yylineno = 0;
    yyparse();

    return parsedStatement;
}

