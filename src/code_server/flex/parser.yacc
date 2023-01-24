%{

// Attempt to use c++ constructs...
// Please work...
#include <iostream>

int yylex(void);
int yynrrer(const char *s);

%}

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

%token NUMBER
%token STRING

%token IDENTIFIER

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

%token OPERATOR_DOT
%token OPERATOR_COMMA
%token OPERATOR_POUND
%token OPERATOR_EXCLAM
%token OPERATOR_COLON
%token OPERATOR_SEMICOLON
%token OPERATOR_OPEN_BRACE
%token OPERATOR_CLOSE_BRACE
%token OPERATOR_OPEN_PAREN
%token OPERATOR_CLOSE_PAREN

%%

START               : FUNCTION
                    | METHOD
                    | DECLARE
                    ;

FUNCTION            : KEYWORD_FUNCTION IDENTIFIER OPERATOR_COLON IDENTIFIER_LIST CODE_BLOCK
                    ;

METHOD              : KEYWORD_METHOD IDENTIFIER OPERATOR_COLON IDENTIFIER_LIST CODE_BLOCK
                    ;

DECLARE             : KEYWORD_DECLARE EXPRESSION OPERATOR_SEMICOLON
                    ;

CODE_BLOCK          : OPERATOR_OPEN_BRACE STATEMENT_LIST OPERATOR_CLOSE_BRACE
                    ;

STATEMENT_LIST      :
                    | STATEMENT STATEMENT_LIST
                    ;

STATEMENT           : FULL_IDENTIFIER OPERATOR_ASSIGN EXPRESSION OPERATOR_SEMICOLON
                    | EXPRESSION OPERATOR_SEMICOLON
                    | KEYWORD_IF EXPRESSION CODE_BLOCK
                    | KEYWORD_WHILE EXPRESSION CODE_BLOCK
                    | KEYWORD_RETURN OPERATOR_SEMICOLON
                    | KEYWORD_RETURN EXPRESSION OPERATOR_SEMICOLON
                    ;

FUNCTION_CALL       : FULL_IDENTIFIER OPERATOR_OPEN_PAREN ARGUMENT_LIST OPERATOR_CLOSE_PAREN
                    ;

ARGUMENT_LIST       :
                    | ARGUMENT_REP
                    ;

ARGUMENT_REP        : EXPRESSION
                    | EXPRESSION OPERATOR_COMMA ARGUMENT_REP
                    ;

IDENTIFIER_LIST     :
                    | IDENTIFIER_REP
                    ;

IDENTIFIER_REP      : IDENTIFIER
                    | IDENTIFIER OPERATOR_COMMA IDENTIFIER_REP
                    ;

FULL_IDENTIFIER     : OPERATOR_POUND IDENTIFIER_PATH
                    | OPERATOR_EXCLAM IDENTIFIER_PATH
                    | IDENTIFIER_PATH
                    ;

IDENTIFIER_PATH     : IDENTIFIER
                    | IDENTIFIER_PATH OPERATOR_DOT IDENTIFIER
                    ;

EXPRESSION          : FUNCTION_CALL
                    | UNARY_BOOL_EXPR
                    ;

UNARY_BOOL_EXPR     : OR_EXPR
                    | KEYWORD_NOT OR_EXPR
                    ;

OR_EXPR             : OR_EXPR KEYWORD_OR AND_EXPR
                    | AND_EXPR
                    ;

AND_EXPR            : AND_EXPR KEYWORD_AND COMP_EXPR
                    | COMP_EXPR
                    ;

COMP_EXPR           : COMP_EXPR OPERATOR_LSS ADD_EXPR
                    | COMP_EXPR OPERATOR_LSEQ ADD_EXPR
                    | COMP_EXPR OPERATOR_GTR ADD_EXPR
                    | COMP_EXPR OPERATOR_GTEQ ADD_EXPR
                    | COMP_EXPR OPERATOR_NEQ ADD_EXPR
                    | COMP_EXPR OPERATOR_EQ ADD_EXPR
                    | ADD_EXPR
                    ;

ADD_EXPR            : ADD_EXPR OPERATOR_ADD MUL_EXPR
                    | ADD_EXPR OPERATOR_SUB MUL_EXPR
                    | MUL_EXPR
                    ;

MUL_EXPR            : MUL_EXPR OPERATOR_MUL UNARY_MINUS
                    | MUL_EXPR OPERATOR_DIV UNARY_MINUS
                    | UNARY_MINUS
                    ;

UNARY_MINUS         : TERMINAL
                    | OPERATOR_SUB TERMINAL
                    ;

TERMINAL            : NUMBER
                    | STRING
                    | KEYWORD_FALSE
                    | KEYWORD_TRUE
                    | KEYWORD_NULL
                    | FULL_IDENTIFIER
                    | OPERATOR_OPEN_PAREN EXPRESSION OPERATOR_CLOSE_PAREN
                    ;

%%

