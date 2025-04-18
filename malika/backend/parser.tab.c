
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "parser.y"

#include <stdio.h>
#include <string.h>
#include "lib/variable.h"
#include "lib/customType.h"
#include "lib/identifier.h"

extern int yylex();
void yyerror(const char *s);

#define YYDEBUG 1
char current_component[256];


/* Line 189 of yacc.c  */
#line 88 "parser.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COMPONENT = 258,
     LBRACE = 259,
     RBRACE = 260,
     LPAREN = 261,
     RPAREN = 262,
     COLON = 263,
     SEMICOLON = 264,
     COMMA = 265,
     LBRACKET = 266,
     RBRACKET = 267,
     EQUALS = 268,
     FUNCTION = 269,
     IMPORT = 270,
     FROM = 271,
     TYPE = 272,
     RETURN = 273,
     LT = 274,
     GT = 275,
     SLASH = 276,
     DOT = 277,
     IDENTIFIER = 278,
     STRING_LITERAL = 279,
     NUMBER_LITERAL = 280,
     BOOLEAN_LITERAL = 281
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 15 "parser.y"

    int intval;   // For numeric values
    char* strval; // For strings like IDENTIFIER



/* Line 214 of yacc.c  */
#line 157 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 169 "parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   83

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  41
/* YYNRULES -- Number of states.  */
#define YYNSTATES  86

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     7,    15,    24,    27,    31,    33,
      36,    38,    40,    44,    48,    55,    62,    73,    84,    89,
      93,    96,   100,   102,   106,   108,   110,   112,   114,   116,
     120,   122,   124,   126,   128,   130,   134,   138,   139,   141,
     143,   147
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      28,     0,    -1,    29,    -1,    30,    -1,     3,    23,     6,
      44,     7,     4,     5,    -1,    14,    23,     6,    44,     7,
       8,    23,    31,    -1,     4,     5,    -1,     4,    32,     5,
      -1,    33,    -1,    33,    32,    -1,    35,    -1,    34,    -1,
      18,    23,     9,    -1,    18,    26,     9,    -1,    23,     8,
      23,    13,    36,     9,    -1,    23,     8,    23,    13,    39,
       9,    -1,    23,     8,    23,    11,    12,    13,    11,    40,
      12,     9,    -1,    23,     8,    23,    11,    12,    13,    11,
      42,    12,     9,    -1,    23,     8,    23,     9,    -1,     4,
      37,     5,    -1,     4,     5,    -1,    37,    10,    38,    -1,
      38,    -1,    23,     8,    39,    -1,    24,    -1,    25,    -1,
      26,    -1,    23,    -1,    41,    -1,    40,    10,    41,    -1,
      25,    -1,    24,    -1,    26,    -1,    23,    -1,    43,    -1,
      42,    10,    43,    -1,     4,    37,     5,    -1,    -1,    45,
      -1,    46,    -1,    45,    10,    46,    -1,    23,     8,    23,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    30,    30,    31,    35,    59,    73,    76,    82,    83,
      97,   101,   109,   129,   136,   268,   335,   376,   426,   462,
     466,   473,   476,   482,   496,   504,   512,   520,   531,   535,
     546,   549,   552,   555,   576,   581,   595,   639,   640,   644,
     646,   657
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMPONENT", "LBRACE", "RBRACE",
  "LPAREN", "RPAREN", "COLON", "SEMICOLON", "COMMA", "LBRACKET",
  "RBRACKET", "EQUALS", "FUNCTION", "IMPORT", "FROM", "TYPE", "RETURN",
  "LT", "GT", "SLASH", "DOT", "IDENTIFIER", "STRING_LITERAL",
  "NUMBER_LITERAL", "BOOLEAN_LITERAL", "$accept", "program", "element",
  "function", "function_content", "instructions", "instruction",
  "return_instruction", "variable_instruction", "field_value_list",
  "field_values", "field_value", "value", "array_values", "array_value",
  "custom_type_array_elements", "custom_type_object", "parameters",
  "typed_param_list", "typed_param", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    27,    28,    28,    29,    30,    31,    31,    32,    32,
      33,    33,    34,    34,    35,    35,    35,    35,    35,    36,
      36,    37,    37,    38,    39,    39,    39,    39,    40,    40,
      41,    41,    41,    41,    42,    42,    43,    44,    44,    45,
      45,    46
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     7,     8,     2,     3,     1,     2,
       1,     1,     3,     3,     6,     6,    10,    10,     4,     3,
       2,     3,     1,     3,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     2,     3,     0,     0,     1,    37,
      37,     0,     0,    38,    39,     0,     0,     0,     0,     0,
      41,     0,    40,     0,     4,     0,     0,     5,     6,     0,
       0,     0,     8,    11,    10,     0,     0,     0,     7,     9,
      12,    13,     0,    18,     0,     0,     0,     0,    27,    24,
      25,    26,     0,     0,     0,    20,     0,     0,    22,    14,
      15,     0,     0,    19,     0,     0,    33,    31,    30,    32,
       0,    28,     0,    34,    23,    21,     0,     0,     0,     0,
       0,    36,    29,    16,    35,    17
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,    27,    31,    32,    33,    34,    52,
      57,    58,    53,    70,    71,    72,    73,    12,    13,    14
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -17
static const yytype_int8 yypact[] =
{
      -2,   -14,     7,    39,   -17,   -17,    37,    38,   -17,    22,
      22,    40,    42,    36,   -17,    43,    24,    47,    22,    44,
     -17,    48,   -17,    31,   -17,    51,     9,   -17,   -17,    15,
      49,    53,   -16,   -17,   -17,    50,    52,    33,   -17,   -17,
     -17,   -17,    20,   -17,    54,    -4,    55,     5,   -17,   -17,
     -17,   -17,    56,    58,    59,   -17,    61,     1,   -17,   -17,
     -17,     0,    -8,   -17,    41,    41,   -17,   -17,   -17,   -17,
      -7,   -17,    30,   -17,   -17,   -17,     3,    11,    62,    68,
      64,   -17,   -17,   -17,   -17,   -17
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -17,   -17,   -17,   -17,   -17,    28,   -17,   -17,   -17,   -17,
      -3,    -1,    12,   -17,     2,   -17,     4,    65,   -17,    60
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      47,     1,    29,    77,    65,    78,    63,    30,    81,     6,
      55,    64,     2,    64,    28,    48,    49,    50,    51,    48,
      49,    50,    51,    66,    67,    68,    69,    29,    56,    43,
       7,    44,    30,    45,    66,    67,    68,    69,    35,     8,
      79,    36,    80,     9,    10,    11,    18,    20,    16,    17,
      19,    21,    23,    24,    25,    26,    42,    37,    38,    40,
      39,    41,    76,    75,    56,    59,    46,    60,    54,    62,
      61,    83,    65,    85,    74,    15,     0,     0,    22,    82,
       0,     0,     0,    84
};

static const yytype_int8 yycheck[] =
{
       4,     3,    18,    10,     4,    12,     5,    23,     5,    23,
       5,    10,    14,    10,     5,    23,    24,    25,    26,    23,
      24,    25,    26,    23,    24,    25,    26,    18,    23,     9,
      23,    11,    23,    13,    23,    24,    25,    26,    23,     0,
      10,    26,    12,     6,     6,    23,    10,    23,     8,     7,
       7,     4,     8,     5,    23,     4,    23,     8,     5,     9,
      32,     9,    65,    64,    23,     9,    12,     9,    13,     8,
      11,     9,     4,     9,    62,    10,    -1,    -1,    18,    77,
      -1,    -1,    -1,    79
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    14,    28,    29,    30,    23,    23,     0,     6,
       6,    23,    44,    45,    46,    44,     8,     7,    10,     7,
      23,     4,    46,     8,     5,    23,     4,    31,     5,    18,
      23,    32,    33,    34,    35,    23,    26,     8,     5,    32,
       9,     9,    23,     9,    11,    13,    12,     4,    23,    24,
      25,    26,    36,    39,    13,     5,    23,    37,    38,     9,
       9,    11,     8,     5,    10,     4,    23,    24,    25,    26,
      40,    41,    42,    43,    39,    38,    37,    10,    12,    10,
      12,     5,    41,     9,    43,     9
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 36 "parser.y"
    { 
          /* $2 is the component name and $4 is the parameter list */
          printf("void %s(%s) {}\n", (yyvsp[(2) - (7)].strval), (yyvsp[(4) - (7)].strval));
          free((yyvsp[(4) - (7)].strval));
      ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 60 "parser.y"
    {
        /* $2 is the function name and $4 is the parameter list */
        printf("%s %s(%s) {\n",(yyvsp[(7) - (8)].strval), (yyvsp[(2) - (8)].strval), (yyvsp[(4) - (8)].strval));
        declare_variables();
        printf("\t%s\n}\n",(yyvsp[(8) - (8)].strval));
        free((yyvsp[(2) - (8)].strval));
        free((yyvsp[(7) - (8)].strval));
        free((yyvsp[(8) - (8)].strval));
        free((yyvsp[(4) - (8)].strval));
    ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 73 "parser.y"
    {
        (yyval.strval) = strdup(""); 
    ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 76 "parser.y"
    {
        (yyval.strval) = (yyvsp[(2) - (3)].strval); 
    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 82 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 83 "parser.y"
    {
        if (strcmp((yyvsp[(1) - (2)].strval)," ") == 0) {
            (yyval.strval) = (yyvsp[(2) - (2)].strval); // Ignore empty instructions
        } else {
            char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
            sprintf(buffer, "%s\n%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
            (yyval.strval) = buffer;
            free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
        }
        
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 97 "parser.y"
    {
        // Générer une instruction de variable
        (yyval.strval) = strdup(" "); // Store the variable name
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 101 "parser.y"
    {
        // Générer une instruction de retour
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // Store the return value
    ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 109 "parser.y"
    {
        // Vérifier si la variable existe
        int found = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, (yyvsp[(2) - (3)].strval)) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Undefined identifier '%s' used in return", (yyvsp[(2) - (3)].strval));
            yyerror(error_msg);
            YYERROR;
        }
        else {
            (yyval.strval) = strdup((yyvsp[(2) - (3)].strval)); // Store the return value
        }
       
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 129 "parser.y"
    {
        (yyval.strval) = strdup((yyvsp[(2) - (3)].strval)); // Store the return value
    ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 136 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (6)].strval);
        char* type_name = (yyvsp[(3) - (6)].strval);
        int is_valid = 1;
        
        // Vérifier d'abord si le type existe
        custom_type* type = find_custom_type(type_name);
        int is_primitive_type = verify_type(type_name);
        
        if (type == NULL && !is_primitive_type) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Vérifier si la variable existe déjà
        if (is_valid && check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        if (is_valid) {
            if (type != NULL) {
                // C'est un type personnalisé - vérifier les attributs et leurs valeurs
                // Créer une table de hachage temporaire pour vérifier les champs fournis
                int field_provided[MAX_FIELDS] = {0}; // Pour marquer les champs fournis
                
                // Vérifier si tous les champs fournis existent dans le type
                for (int i = 0; i < field_count && is_valid; i++) {
                    int field_found = 0;
                    
                    for (int j = 0; j < type->field_count; j++) {
                        if (strcmp(field_names[i], type->fields[j][0]) == 0) {
                            field_found = 1;
                            field_provided[j] = 1; // Marquer ce champ comme fourni
                            
                            // Vérifier la compatibilité du type pour ce champ
                            if (!check_field_value_compatibility(type->fields[j][1], field_values[i], field_types[i])) {
                                char error_msg[256];
                                snprintf(error_msg, sizeof(error_msg), 
                                       "Type error: Cannot assign '%s' to field '%s' of type '%s'",
                                       field_values[i], field_names[i], type->fields[j][1]);
                                yyerror(error_msg);
                                is_valid = 0;
                            }
                            break;
                        }
                    }
                    
                    if (!field_found) {
                        char error_msg[256];
                        snprintf(error_msg, sizeof(error_msg), 
                               "Error: Field '%s' does not exist in type '%s'",
                               field_names[i], type_name);
                        yyerror(error_msg);
                        is_valid = 0;
                    }
                }
                
                // Vérifier que tous les champs requis sont fournis
                for (int j = 0; j < type->field_count && is_valid; j++) {
                    if (!field_provided[j] && type->fields[j][2] != NULL && strcmp(type->fields[j][2], "required") == 0) {
                        char error_msg[256];
                        snprintf(error_msg, sizeof(error_msg), 
                               "Error: Required field '%s' of type '%s' is missing",
                               type->fields[j][0], type_name);
                        yyerror(error_msg);
                        is_valid = 0;
                    }
                }
                
                if (is_valid) {
                    char value_buffer[500] = "{";
                    for (int i = 0; i < field_count; i++) {
                        char field_entry[128];
                        
                        // Ajouter des guillemets autour des strings
                        if (field_types[i]== 0) {
                            snprintf(field_entry, sizeof(field_entry), "%s=\"%s\"", field_names[i], field_values[i]);
                        } else {
                            snprintf(field_entry, sizeof(field_entry), "%s=%s", field_names[i], field_values[i]);
                        }

                        strcat(value_buffer, field_entry);
                        if (i < field_count - 1) {
                            strcat(value_buffer, ", ");
                        }
                    }
                    strcat(value_buffer, "}");

                    // Appel existant (inchangé) avec la vraie valeur maintenant
                    add_variable(var_name, type_name, value_buffer, 0);

                }
            } else if (is_primitive_type) {
                // C'est un type primitif
                if (field_count != 1) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Error: Primitive type '%s' expects single value, got %d values", 
                           type_name, field_count);
                    yyerror(error_msg);
                    is_valid = 0;
                } else if (!check_primitive_type_compatibility(type_name, field_values[0], field_types[0])) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Type error: Cannot assign '%s' to variable of type '%s'", 
                           field_values[0], type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                } else {
                    // Ajouter la variable primitive
                    add_variable(var_name, type_name, field_values[0], 0); // 0 = type primitif
                }
            }
        }
        
        // Réinitialiser les compteurs
        field_count = 0;
        value_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        (yyval.strval) = strdup(var_name); // Return the variable name for further processing if needed
        free(var_name);
        free(type_name);
    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 268 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (6)].strval);
        char* type_name = (yyvsp[(3) - (6)].strval);
        int is_valid = 1;
        char* declaration= NULL;
        // Vérifier que la valeur a bien été ajoutée
        if (value_count != 1) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Expected 1 value, got %d", value_count);
            yyerror(error_msg);
            is_valid = 0;
        } else {
            char* value_str = field_values[0];
            // Vérifier si la variable existe déjà
            if (check_variable_exists(var_name)) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
                yyerror(error_msg);
                is_valid = 0;
            }
            
            if (is_valid) {
                // Vérifier si c'est un type personnalisé
                custom_type* type = find_custom_type(type_name);
                if (type != NULL) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Error: Custom type '%s' requires %d values", type_name, type->field_count);
                    yyerror(error_msg);
                    is_valid = 0;
                } 
                // Vérifier si c'est un type primitif valide
                else if (!verify_type(type_name)) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                }
                // Vérifier la compatibilité des types
                else if (!check_primitive_type_compatibility(type_name, value_str, field_types[0])) {
                    char error_msg[256];
                    snprintf(error_msg, sizeof(error_msg), 
                           "Type error: Cannot assign '%s' to variable of type '%s'", 
                           value_str, type_name);
                    yyerror(error_msg);
                    is_valid = 0;
                }
                else {
                    // Ajouter la variable
                    add_variable(var_name, type_name, value_str, 0); // 0 = type primitif
                    
                }
            }
        }
        
        // Réinitialiser le compteur de valeurs
        value_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        (yyval.strval) = strdup(var_name);
        free(var_name);
        free(type_name);
    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 335 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (10)].strval);
        char* type_name = (yyvsp[(3) - (10)].strval);
        
        // Check if variable already exists
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, var_name) == 0) {
                char error_msg[256];
                snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
                yyerror(error_msg);
                YYERROR;
                break;
            }
        }
        
        // Verify the base type exists
        if (verify_type(type_name)) {
            // Add variable with is_array flag set to 1
            char array_value_str[1024] = "{";
            for (int i = 0; i < array_value_count; i++) {
                strcat(array_value_str, array_values[i]);
                if (i < array_value_count - 1) {
                    strcat(array_value_str, ", ");
                }
            }
            strcat(array_value_str, "}");
            add_variable(var_name, type_name, array_value_str, 1);
            (yyval.strval) = strdup(var_name);
            
            // Reset array_value_count
            array_value_count = 0;
        } else {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Type '%s' is not defined", type_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        free(var_name);
        free(type_name);
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 376 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (10)].strval);
        char* type_name = (yyvsp[(3) - (10)].strval);
        int is_valid = 1;
        
        // Check if the type exists and is a custom type
        custom_type* type = find_custom_type(type_name);
        if (type == NULL) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Custom type '%s' is not defined", type_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Check if variable already exists
        if (is_valid && check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Variable '%s' already declared", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        if (is_valid) {
            // Format the array of custom type objects
            char array_value[2048] = "{"; // Larger buffer for complex structures
            
            for (int i = 0; i < custom_array_element_count; i++) {
                strcat(array_value, custom_array_elements[i]);
                
                if (i < custom_array_element_count - 1) {
                    strcat(array_value, ", ");
                }
            }
            strcat(array_value, "}");
            
            // Add the array variable
            add_variable(var_name, type_name, array_value, 1); // 1 = is_array
        }
        
        // Reset array element count
        custom_array_element_count = 0;
        
        if (!is_valid) {
            YYERROR;
        }
        
        (yyval.strval) = strdup(var_name);
        free(var_name);
        free(type_name);
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 427 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (4)].strval);
        char* type_name = (yyvsp[(3) - (4)].strval);
        int is_valid = 1;
        
        // Vérifier si la variable existe déjà
        if (check_variable_exists(var_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),"Variable '%s' already declared" , var_name);
            yyerror(error_msg); 
            is_valid = 0;
        }
        
        // Vérifier si c'est un type valide
        if (is_valid && !verify_type(type_name)) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg),"Type '%s' is not defined", var_name);
            yyerror(error_msg);
            is_valid = 0;
        }
        
        // Ajouter la variable si tout est valide
        if (is_valid) {
            add_variable(var_name, type_name, get_default_value(type_name), 0);
        }
        
        if (!is_valid) {
            YYERROR;
        }
        
        (yyval.strval) = var_name; // Pas besoin de strdup car var_name sera utilisé
    ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 462 "parser.y"
    {
        // Traitement terminé, résultat déjà stocké dans field_names et field_values
        (yyval.strval) = strdup(""); // Simplement pour éviter les erreurs de syntaxe
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 466 "parser.y"
    {
        // Cas d'un objet vide
        (yyval.strval) = strdup("");
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 473 "parser.y"
    {
        // Ajoute simplement une nouvelle paire field_name:value
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 476 "parser.y"
    {
        // Premier champ
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 482 "parser.y"
    {
        // Stocker le nom du champ
        if (field_count < MAX_FIELDS) {
            strcpy(field_names[field_count], (yyvsp[(1) - (3)].strval));
            // La valeur a déjà été stockée dans field_values par la règle value
            field_count++;
        } else {
            yyerror("Too many fields");
        }
        free((yyvsp[(1) - (3)].strval));
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 496 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_STRING;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 504 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_NUMBER;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 512 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_BOOLEAN;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 520 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_IDENTIFIER;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 531 "parser.y"
    {
        array_values[0] = (yyvsp[(1) - (1)].strval);
        array_value_count = 1;
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 535 "parser.y"
    {
        if (array_value_count < MAX_ARRAY_VALUES) {
            array_values[array_value_count++] = (yyvsp[(3) - (3)].strval);
        } else {
            yyerror("Too many array values");
            YYERROR;
        }
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 546 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 549 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 552 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 555 "parser.y"
    {
        // Check if the identifier exists
        int found = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, (yyvsp[(1) - (1)].strval)) == 0) {
                found = 1;
                (yyval.strval) = strdup(variables[i].name);
                break;
            }
        }
        if (!found) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "Error: Undefined identifier '%s' used in array", (yyvsp[(1) - (1)].strval));
            yyerror(error_msg);
            YYERROR;
        }
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 576 "parser.y"
    {
        strcpy(custom_array_elements[0], (yyvsp[(1) - (1)].strval));
        custom_array_element_count = 1;
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 581 "parser.y"
    {
        if (custom_array_element_count < MAX_ARRAY_ELEMENTS) {
            strcpy(custom_array_elements[custom_array_element_count], (yyvsp[(3) - (3)].strval));
            custom_array_element_count++;
        } else {
            yyerror("Too many array elements");
            YYERROR;
        }
        free((yyvsp[(3) - (3)].strval));
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 595 "parser.y"
    {
        // Create a temporary buffer for the object
        char object_str[1024] = "{";
        int len = 0;
        
        for (int i = 0; i < field_count; i++) {
            // Format each field correctly
            char field_entry[256];
            
            // Handle string value formatting (adding quotes if necessary)
            if (field_types[i] == TYPE_STRING) {
                // Check if already quoted
                if (field_values[i][0] != '"') {
                    snprintf(field_entry, sizeof(field_entry), "%s = \"%s\"", 
                             field_names[i], field_values[i]);
                } else {
                    snprintf(field_entry, sizeof(field_entry), "%s = %s", 
                             field_names[i], field_values[i]);
                }
            } else {
                // For non-string types
                snprintf(field_entry, sizeof(field_entry), "%s = %s", 
                         field_names[i], field_values[i]);
            }
            
            // Add to the object string
            strcat(object_str, field_entry);
            if (i < field_count - 1) {
                strcat(object_str, ", ");
            }
        }
        
        strcat(object_str, "}");
        
        // Store this object and reset field counters
        (yyval.strval) = strdup(object_str);
        
        // Reset field_count and value_count for the next object
        field_count = 0;
        value_count = 0;
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 639 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 640 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 645 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 647 "parser.y"
    {
          /* Concatenate the previous list with ", " and the new parameter */
          char* tmp = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 3); // extra space for comma, space, and '\0'
          sprintf(tmp, "%s, %s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
          free((yyvsp[(1) - (3)].strval));
          (yyval.strval) = tmp;
      ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 658 "parser.y"
    {
        /* The DSL expects parameters as "var : type".
        In C, parameters are declared as "type var". 
        So $1 is the variable name and $3 is its type.
        */
        char* res = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 2);
        sprintf(res, "%s %s", (yyvsp[(3) - (3)].strval), (yyvsp[(1) - (3)].strval));
        (yyval.strval) = res;
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2215 "parser.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 668 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


int main() {
    yydebug = 1;
    int result = yyparse();
    if (result == 0) {
        printf( "✅ OK\n");
    } else {
        printf( "❌ Erreur\n ");
    }
    return 0;
}   
