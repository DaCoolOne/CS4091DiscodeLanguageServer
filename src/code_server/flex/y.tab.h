
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KEYWORD_IF = 258,
     KEYWORD_WHILE = 259,
     KEYWORD_RETURN = 260,
     KEYWORD_DECLARE = 261,
     KEYWORD_FUNCTION = 262,
     KEYWORD_METHOD = 263,
     KEYWORD_NULL = 264,
     KEYWORD_TRUE = 265,
     KEYWORD_FALSE = 266,
     KEYWORD_AND = 267,
     KEYWORD_OR = 268,
     KEYWORD_NOT = 269,
     NUMBER = 270,
     STRING = 271,
     IDENTIFIER = 272,
     OPERATOR_EQ = 273,
     OPERATOR_NEQ = 274,
     OPERATOR_LSEQ = 275,
     OPERATOR_GTEQ = 276,
     OPERATOR_ASSIGN = 277,
     OPERATOR_ADD = 278,
     OPERATOR_SUB = 279,
     OPERATOR_MUL = 280,
     OPERATOR_DIV = 281,
     OPERATOR_LSS = 282,
     OPERATOR_GTR = 283,
     OPERATOR_DOT = 284,
     OPERATOR_COMMA = 285,
     OPERATOR_POUND = 286,
     OPERATOR_EXCLAM = 287,
     OPERATOR_COLON = 288,
     OPERATOR_SEMICOLON = 289,
     OPERATOR_OPEN_BRACE = 290,
     OPERATOR_CLOSE_BRACE = 291,
     OPERATOR_OPEN_PAREN = 292,
     OPERATOR_CLOSE_PAREN = 293
   };
#endif
/* Tokens.  */
#define KEYWORD_IF 258
#define KEYWORD_WHILE 259
#define KEYWORD_RETURN 260
#define KEYWORD_DECLARE 261
#define KEYWORD_FUNCTION 262
#define KEYWORD_METHOD 263
#define KEYWORD_NULL 264
#define KEYWORD_TRUE 265
#define KEYWORD_FALSE 266
#define KEYWORD_AND 267
#define KEYWORD_OR 268
#define KEYWORD_NOT 269
#define NUMBER 270
#define STRING 271
#define IDENTIFIER 272
#define OPERATOR_EQ 273
#define OPERATOR_NEQ 274
#define OPERATOR_LSEQ 275
#define OPERATOR_GTEQ 276
#define OPERATOR_ASSIGN 277
#define OPERATOR_ADD 278
#define OPERATOR_SUB 279
#define OPERATOR_MUL 280
#define OPERATOR_DIV 281
#define OPERATOR_LSS 282
#define OPERATOR_GTR 283
#define OPERATOR_DOT 284
#define OPERATOR_COMMA 285
#define OPERATOR_POUND 286
#define OPERATOR_EXCLAM 287
#define OPERATOR_COLON 288
#define OPERATOR_SEMICOLON 289
#define OPERATOR_OPEN_BRACE 290
#define OPERATOR_CLOSE_BRACE 291
#define OPERATOR_OPEN_PAREN 292
#define OPERATOR_CLOSE_PAREN 293




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 10 "c:\\Users\\_DaCoolOne_\\Documents\\GitHub\\DiscodeLanguageServer\\src\\code_server\\flex\\parser.yacc"

    AST_Node* node;
    char* string;



/* Line 1676 of yacc.c  */
#line 135 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


