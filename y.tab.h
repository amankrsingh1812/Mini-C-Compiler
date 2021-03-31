/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    ID = 259,
    num = 260,
    LPARENTHESIS = 261,
    RPARENTHESIS = 262,
    RCURLY = 263,
    LCURLY = 264,
    RETURN = 265,
    SEMICOLON = 266,
    ADDITION = 267,
    MINUS = 268,
    MULTIPLICATION = 269,
    DIVISION = 270,
    COMPLEMENT = 271,
    LOGICAL_NEGATION = 272,
    AND = 273,
    OR = 274,
    EQUAL = 275,
    NOTEQUAL = 276,
    LESSTHAN = 277,
    LESSTHANEQ = 278,
    GREATERTHAN = 279,
    GREATERTHANEQ = 280,
    MAIN = 281,
    VOID = 282
  };
#endif
/* Tokens.  */
#define INT 258
#define ID 259
#define num 260
#define LPARENTHESIS 261
#define RPARENTHESIS 262
#define RCURLY 263
#define LCURLY 264
#define RETURN 265
#define SEMICOLON 266
#define ADDITION 267
#define MINUS 268
#define MULTIPLICATION 269
#define DIVISION 270
#define COMPLEMENT 271
#define LOGICAL_NEGATION 272
#define AND 273
#define OR 274
#define EQUAL 275
#define NOTEQUAL 276
#define LESSTHAN 277
#define LESSTHANEQ 278
#define GREATERTHAN 279
#define GREATERTHANEQ 280
#define MAIN 281
#define VOID 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
