%{
#include "y.tab.h"
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
\"[^\"]+\"              { return STRING; }
[0-9]+                  { return NUMBER; }
\w+                     { return IDENTIFIER; }

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
\(                      { return OPERATOR_OPEN_PAREN; }
\)                      { return OPERATOR_CLOSE_PAREN; }

\s                      {}


