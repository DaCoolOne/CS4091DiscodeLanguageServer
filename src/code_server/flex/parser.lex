%option noyywrap yylineno

%{
#include "flex/ast.h"
#include "flex/y.tab.h"
#include <stdio.h>

#define YY_DECL int yylex()
%}

%%

if                      { return KEYWORD_IF; }
while                   { return KEYWORD_WHILE; }
return                  { return KEYWORD_RETURN; }
declare                 { return KEYWORD_DECLARE; }
function                { return KEYWORD_FUNCTION; }
method                  { return KEYWORD_METHOD; }
null                    { return KEYWORD_NULL; }
true                    { return KEYWORD_TRUE; }
false                   { return KEYWORD_FALSE; }
and                     { return KEYWORD_AND; }
or                      { return KEYWORD_OR; }
not                     { return KEYWORD_NOT; }

    /* Todo: Make these regexes better */
\"(\\.|[^\\\"])+\"      { yylval.node = createString(yytext); return STRING; }
\'(\\.|[^\\\'])+\'      { yylval.node = createString(yytext); return STRING; }
[0-9]+                  { yylval.node = createNumber(yytext); return NUMBER; }
[0-9]+\.[0-9]           { yylval.node = createNumber(yytext); return NUMBER; }
[A-Za-z_][A-Za-z0-9_]*  { yylval.node = createIdentifier(yytext); return IDENTIFIER; }

==                      { return OPERATOR_EQ; }
!=                      { return OPERATOR_NEQ; }
\<=                     { return OPERATOR_LSEQ; }
\>=                     { return OPERATOR_GTEQ; }

\<\-                    { return OPERATOR_ASSIGN; }

\+                      { return OPERATOR_ADD; }
\-                      { return OPERATOR_SUB; }
\*                      { return OPERATOR_MUL; }
\/                      { return OPERATOR_DIV; }
\<                      { return OPERATOR_LSS; }
\>                      { return OPERATOR_GTR; }

\.                      { return OPERATOR_DOT; }
,                       { return OPERATOR_COMMA; }
#                       { return OPERATOR_POUND; }
!                       { return OPERATOR_EXCLAM; }
:                       { return OPERATOR_COLON; }
\;                      { return OPERATOR_SEMICOLON; }
\{                      { return OPERATOR_OPEN_BRACE; }
\}                      { return OPERATOR_CLOSE_BRACE; }
\[                      { return OPERATOR_OPEN_SQUARE; }
\]                      { return OPERATOR_CLOSE_SQUARE; }
\(                      { return OPERATOR_OPEN_PAREN; }
\)                      { return OPERATOR_CLOSE_PAREN; }

\s+                     {  }
\n                      {}

%%

void set_yyin(FILE * new_yyin) {
    yyin = new_yyin;
}

int feof_yyin() {
    return feof(yyin);
}

