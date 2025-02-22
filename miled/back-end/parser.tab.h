
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
     VOID = 258,
     CHAR = 259,
     SHORT = 260,
     INT_TYPE = 261,
     LONG = 262,
     FLOAT = 263,
     DOUBLE = 264,
     BOOL_TYPE = 265,
     UNSIGNED = 266,
     SIGNED = 267,
     STRUCT = 268,
     UNION = 269,
     ENUM = 270,
     TYPEDEF = 271,
     CONST = 272,
     STATIC = 273,
     EXTERN = 274,
     REGISTER = 275,
     VOLATILE = 276,
     FUNCTION = 277,
     RETURN = 278,
     TRUE = 279,
     FALSE = 280,
     AND_OP = 281,
     OR_OP = 282,
     EQ_OP = 283,
     NE_OP = 284,
     LE_OP = 285,
     GE_OP = 286,
     IDENTIFIER = 287,
     STRING_LITERAL = 288,
     INT_LITERAL = 289,
     FLOAT_LITERAL = 290
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 15 "parser.y"

    int intVal;
    float floatVal;
    char *str;



/* Line 1676 of yacc.c  */
#line 95 "parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


