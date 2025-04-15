
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "lib/component.h"
#include "lib/customType.h"
#include "lib/variable.h"
#include "lib/identifier.h"

extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;  
extern void yyrestart(FILE* input_file);
#define YYDEBUG 1


char output_buffer[10000];
char interfaces_buffer[1000];  // Buffer pour les interfaces TypeScript
// int id_count = 0;
int found = 0;

typedef struct {
    FILE* yyin_backup;
    char output_buffer_backup[10000];
    char current_component_name_backup[100];
    TypedIdentifier identifiers_backup[100];
    int id_count_backup;
} ParserState;



void append_to_buffer(const char *str) {
    // Make sure we don't overflow the buffer
    size_t current_len = strlen(output_buffer);
    size_t str_len = strlen(str);
    
    if (current_len + str_len < sizeof(output_buffer) - 1) {
        strcat(output_buffer, str);
    } else {
        fprintf(stderr, "Warning: Output buffer overflow prevented\n");
    }
}

// Fonction pour générer un fichier HTML avec le contenu du buffer
void generate_html(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open file %s for writing\n", filename);
        return;
    }
    
    // Écrire l'en-tête HTML standard
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <title>Generated Component</title>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    
    // Écrire le contenu du buffer
    fprintf(file, "%s\n", output_buffer);
    
    // Fermer le document HTML
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");
    
    fclose(file);
    printf("HTML file generated successfully: %s\n", filename);
}

// // Fonction pour générer le code HTML pur selon le type
char* generate_html_code(const char* id_name) {
    char* id_type = get_identifier_type(id_name);
    char* buffer = malloc(1000);

    if (id_type != NULL) {
        if (strcmp(id_type, "number") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else if (strcmp(id_type, "boolean") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else if (strcmp(id_type, "string") == 0) {
            sprintf(buffer, "{%s}", id_name);
        } else {
            // Type complexe ou inconnu
            sprintf(buffer, "{%s}", id_name);
        }
    } else {
        // Type non trouvé, traite comme un littéral de texte
        sprintf(buffer, "{%s}", id_name);
    }
    
    return buffer;
}

// Fonction pour libérer la mémoire des identifiants
void liberer_pile() {
    for (int i = 0; i < id_count; i++) {
        free(identifiers[i].name);
        free(identifiers[i].type);
    }
    id_count = 0;
}



/* Line 189 of yacc.c  */
#line 183 "parser.tab.c"

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
     RETURN = 259,
     EQUALS = 260,
     LBRACE = 261,
     RBRACE = 262,
     LPAREN = 263,
     RPAREN = 264,
     COLON = 265,
     TYPE = 266,
     SEMICOLON = 267,
     FROM = 268,
     IMPORT = 269,
     COMMA = 270,
     LT = 271,
     GT = 272,
     SLASH = 273,
     DOT = 274,
     LBRACKET = 275,
     RBRACKET = 276,
     IDENTIFIER = 277,
     STRING_LITERAL = 278,
     NUMBER_LITERAL = 279,
     BOOLEAN_LITERAL = 280
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 110 "parser.y"

    int intval;
    char* strval;



/* Line 214 of yacc.c  */
#line 251 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 263 "parser.tab.c"

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   156

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  26
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNRULES -- Number of states.  */
#define YYNSTATES  130

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

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
      25
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    15,    16,    17,    19,    23,
      27,    31,    32,    34,    36,    39,    41,    43,    45,    46,
      55,    58,    60,    65,    71,    75,    78,    82,    84,    88,
      90,    92,    94,    96,   103,   110,   121,   132,   134,   138,
     140,   142,   144,   146,   148,   152,   156,   158,   162,   167,
     174,   177,   179,   185,   186,   189,   192,   194,   204,   210,
     212,   213,   216,   220,   222
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      27,     0,    -1,    28,    -1,    50,    28,    -1,     3,    22,
       8,    29,     9,    31,    -1,    -1,    -1,    30,    -1,    30,
      15,    29,    -1,    22,    10,    22,    -1,     6,    32,     7,
      -1,    -1,    33,    -1,    34,    -1,    34,    33,    -1,    35,
      -1,    44,    -1,    39,    -1,    -1,    11,    22,     5,    36,
       6,    37,     7,    12,    -1,    37,    38,    -1,    38,    -1,
      22,    10,    22,    12,    -1,     4,     8,    52,     9,    12,
      -1,     6,    41,     7,    -1,     6,     7,    -1,    41,    15,
      42,    -1,    42,    -1,    22,    10,    43,    -1,    23,    -1,
      24,    -1,    25,    -1,    22,    -1,    22,    10,    22,     5,
      40,    12,    -1,    22,    10,    22,     5,    43,    12,    -1,
      22,    10,    22,    20,    21,     5,    20,    45,    21,    12,
      -1,    22,    10,    22,    20,    21,     5,    20,    47,    21,
      12,    -1,    46,    -1,    45,    15,    46,    -1,    24,    -1,
      23,    -1,    25,    -1,    22,    -1,    48,    -1,    47,    15,
      48,    -1,     6,    41,     7,    -1,    22,    -1,    22,    19,
      22,    -1,    22,    20,    24,    21,    -1,    22,    20,    24,
      21,    19,    22,    -1,    50,    51,    -1,    51,    -1,    14,
      22,    13,    23,    12,    -1,    -1,    52,    53,    -1,    52,
      56,    -1,    53,    -1,    16,    22,    54,    17,    52,    16,
      18,    22,    17,    -1,    16,    22,    54,    18,    17,    -1,
      56,    -1,    -1,    55,    54,    -1,    22,     5,    23,    -1,
      22,    -1,     6,    49,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   126,   126,   202,   280,   300,   304,   305,   306,   315,
     325,   331,   332,   338,   339,   353,   357,   361,   369,   369,
     378,   385,   389,   401,   406,   410,   417,   420,   426,   440,
     448,   456,   464,   475,   607,   674,   716,   770,   774,   785,
     788,   791,   794,   815,   820,   834,   878,   902,   928,   972,
    1047,  1054,  1058,  1069,  1070,  1077,  1083,  1089,  1148,  1193,
    1199,  1200,  1209,  1218,  1245
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMPONENT", "RETURN", "EQUALS",
  "LBRACE", "RBRACE", "LPAREN", "RPAREN", "COLON", "TYPE", "SEMICOLON",
  "FROM", "IMPORT", "COMMA", "LT", "GT", "SLASH", "DOT", "LBRACKET",
  "RBRACKET", "IDENTIFIER", "STRING_LITERAL", "NUMBER_LITERAL",
  "BOOLEAN_LITERAL", "$accept", "program", "element", "parameters",
  "parameter", "function", "function_body", "instructions", "instruction",
  "type_instruction", "$@1", "type_properties", "type_property",
  "return_instruction", "field_value_list", "field_values", "field_value",
  "value", "variable_instruction", "array_values", "array_value",
  "custom_type_array_elements", "custom_type_object",
  "identifiant_interpole", "import_instructions", "import_instruction",
  "html_content", "html_element", "attributes", "attribute", "html_inner", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    26,    27,    27,    28,    28,    29,    29,    29,    30,
      31,    32,    32,    33,    33,    34,    34,    34,    36,    35,
      37,    37,    38,    39,    40,    40,    41,    41,    42,    43,
      43,    43,    43,    44,    44,    44,    44,    45,    45,    46,
      46,    46,    46,    47,    47,    48,    49,    49,    49,    49,
      50,    50,    51,    52,    52,    52,    52,    53,    53,    53,
      54,    54,    55,    56,    56
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     6,     0,     0,     1,     3,     3,
       3,     0,     1,     1,     2,     1,     1,     1,     0,     8,
       2,     1,     4,     5,     3,     2,     3,     1,     3,     1,
       1,     1,     1,     6,     6,    10,    10,     1,     3,     1,
       1,     1,     1,     1,     3,     3,     1,     3,     4,     6,
       2,     1,     5,     0,     2,     2,     1,     9,     5,     1,
       0,     2,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     0,     0,     2,     5,    51,     0,     0,     1,
       3,    50,     6,     0,     0,     0,     7,     0,     0,     0,
       6,    52,     9,    11,     4,     8,     0,     0,     0,     0,
      12,    13,    15,    17,    16,    53,     0,     0,    10,    14,
       0,     0,    63,     0,    56,    59,    18,     0,    46,     0,
      60,     0,    54,    55,     0,     0,     0,     0,     0,    64,
       0,     0,    60,    23,     0,     0,    32,    29,    30,    31,
       0,     0,     0,    47,     0,     0,     0,     0,    61,     0,
       0,    21,    25,     0,     0,    27,    33,    34,     0,    48,
      62,     0,    58,     0,     0,    20,     0,    24,     0,     0,
       0,     0,     0,    19,    28,    26,     0,    42,    40,    39,
      41,     0,    37,     0,    43,    49,     0,    22,     0,     0,
       0,     0,     0,     0,    45,    38,    35,    44,    36,    57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    15,    16,    24,    29,    30,    31,    32,
      54,    80,    81,    33,    70,    84,    85,    71,    34,   111,
     112,   113,   114,    49,     5,     6,    43,    44,    61,    62,
      45
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -44
static const yytype_int8 yypact[] =
{
      33,    -8,    40,    46,   -44,    33,   -44,     2,    20,   -44,
     -44,   -44,    42,    32,    53,    56,    51,    55,    47,    62,
      42,   -44,   -44,     4,   -44,   -44,    63,    48,    64,    65,
     -44,     4,   -44,   -44,   -44,    22,    68,    54,   -44,   -44,
      57,    58,   -44,    21,   -44,   -44,   -44,    11,    41,    70,
      59,    66,   -44,   -44,    69,    -4,    61,    67,    60,   -44,
      78,    24,    59,   -44,    71,    10,   -44,   -44,   -44,   -44,
      73,    74,    82,   -44,    75,    72,    22,    77,   -44,    80,
      28,   -44,   -44,    81,    -3,   -44,   -44,   -44,    79,    83,
     -44,    23,   -44,    76,    85,   -44,    29,   -44,    84,     0,
      86,   -11,    88,   -44,   -44,   -44,    84,   -44,   -44,   -44,
     -44,   -12,   -44,    19,   -44,   -44,    87,   -44,    -2,    34,
      89,    97,    92,    90,   -44,   -44,   -44,   -44,   -44,   -44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -44,   -44,   100,    91,   -44,   -44,   -44,    93,   -44,   -44,
     -44,   -44,     8,   -44,   -44,   -14,    12,    16,   -44,   -44,
      -6,   -44,    -7,   -44,   -44,   110,    43,   -43,    94,   -44,
     -42
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      52,    53,    65,   119,    97,   124,   106,   116,    26,   120,
      12,    50,    98,    98,     7,    27,    55,    82,    66,    67,
      68,    69,   107,   108,   109,   110,    28,    40,    40,    40,
      51,    56,    83,    13,   121,    94,     1,    41,    41,   101,
     122,    76,    77,    42,    42,    42,     9,     2,    52,    53,
      79,    66,    67,    68,    69,    17,   107,   108,   109,   110,
      57,    58,     8,    18,    14,    19,    20,    21,    23,    22,
      36,    35,    38,    46,    37,    64,    47,    59,    63,    48,
      50,    60,    72,    75,    74,    86,    87,    88,    95,    73,
      93,    96,   118,    79,    92,    90,    89,   103,   102,    99,
     117,   126,   100,   106,   128,    10,    83,   129,   115,   123,
     105,    25,   104,   125,   127,    11,     0,     0,     0,    91,
       0,     0,     0,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    78
};

static const yytype_int8 yycheck[] =
{
      43,    43,     6,    15,     7,     7,     6,    18,     4,    21,
       8,    22,    15,    15,    22,    11,     5,     7,    22,    23,
      24,    25,    22,    23,    24,    25,    22,     6,     6,     6,
       9,    20,    22,    13,    15,     7,     3,    16,    16,    16,
      21,    17,    18,    22,    22,    22,     0,    14,    91,    91,
      22,    22,    23,    24,    25,    23,    22,    23,    24,    25,
      19,    20,    22,    10,    22,     9,    15,    12,     6,    22,
      22,     8,     7,     5,    10,     6,    22,     7,    12,    22,
      22,    22,    21,     5,    24,    12,    12,     5,    80,    22,
      10,    10,   106,    22,    17,    23,    21,    12,    22,    20,
      12,    12,    19,     6,    12,     5,    22,    17,    22,    22,
      98,    20,    96,   119,   121,     5,    -1,    -1,    -1,    76,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    14,    27,    28,    50,    51,    22,    22,     0,
      28,    51,     8,    13,    22,    29,    30,    23,    10,     9,
      15,    12,    22,     6,    31,    29,     4,    11,    22,    32,
      33,    34,    35,    39,    44,     8,    22,    10,     7,    33,
       6,    16,    22,    52,    53,    56,     5,    22,    22,    49,
      22,     9,    53,    56,    36,     5,    20,    19,    20,     7,
      22,    54,    55,    12,     6,     6,    22,    23,    24,    25,
      40,    43,    21,    22,    24,     5,    17,    18,    54,    22,
      37,    38,     7,    22,    41,    42,    12,    12,     5,    21,
      23,    52,    17,    10,     7,    38,    10,     7,    15,    20,
      19,    16,    22,    12,    43,    42,     6,    22,    23,    24,
      25,    45,    46,    47,    48,    22,    18,    12,    41,    15,
      21,    15,    21,    22,     7,    46,    12,    48,    12,    17
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
        case 2:

/* Line 1455 of yacc.c  */
#line 126 "parser.y"
    {
        liberer_pile();
        // Before calling generate_structs_and_prototypes()
        printf("// Définition des types personnalisés\n");
        fflush(stdout);

        // Call the function
        generate_structs_and_prototypes(); 
        
        // After calling
        fflush(stdout);
        // Écrire les en-têtes nécessaires
        printf("#include <stdio.h>\n");
        printf("#include <stdlib.h>\n");
        printf("#include <string.h>\n\n");
        
        
        // Déclarer le buffer global
        printf("char output_buffer[10000] = {\n");
        
        // Remplir le buffer avec le contenu HTML généré
        printf("    \"");
        for (size_t i = 0; i < strlen(output_buffer); i++) {
            if (output_buffer[i] == '\n') {
                printf("\\n\"\n    \"");
            } else if (output_buffer[i] == '"') {
                printf("\\\"");
            } else if (output_buffer[i] == '\\') {
                printf("\\\\");
            } else {
                printf("%c", output_buffer[i]);
            }
        }
        printf("\"\n};\n\n");
        
        // Écrire la fonction generate_html
        printf("// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
        printf("void generate_html(const char *filename) {\n");
        printf("    FILE *file = fopen(filename, \"w\");\n");
        printf("    if (file == NULL) {\n");
        printf("        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
        printf("        return;\n");
        printf("    }\n");
        printf("    \n");
        printf("    // Écrire l'en-tête HTML standard\n");
        printf("    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
        printf("    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
        printf("    fprintf(file, \"<head>\\n\");\n");
        printf("    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
        printf("    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
        printf("    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
        printf("    fprintf(file, \"</head>\\n\");\n");
        printf("    fprintf(file, \"<body>\\n\");\n");
        printf("    \n");
        printf("    // Écrire le contenu du buffer\n");
        printf("    fprintf(file, \"%%s\\n\", output_buffer);\n");
        printf("    \n");
        printf("    // Fermer le document HTML\n");
        printf("    fprintf(file, \"</body>\\n\");\n");
        printf("    fprintf(file, \"</html>\\n\");\n");
        printf("    \n");
        printf("    fclose(file);\n");
        printf("    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
        printf("}\n\n");
        
        // Ajouter une fonction main pour tester
        printf("int main(int argc, char *argv[]) {\n");
        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        printf("\n");
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 202 "parser.y"
    {
        liberer_pile();
        
        // Écrire les en-têtes nécessaires
        printf("#include <stdio.h>\n");
        printf("#include <stdlib.h>\n");
        printf("#include <string.h>\n\n");
        
        // Before calling generate_structs_and_prototypes()
        printf("// Définition des types personnalisés\n");
        

        // Call the function
        generate_structs_and_prototypes();

        
        // Déclarer le buffer global
        printf("\nchar output_buffer[10000] = {\n");
        
        // Remplir le buffer avec le contenu HTML généré
        printf("    \"");
        for (size_t i = 0; i < strlen(output_buffer); i++) {
            if (output_buffer[i] == '\n') {
                printf("\\n\"\n    \"");
            } else if (output_buffer[i] == '"') {
                printf("\\\"");
            } else if (output_buffer[i] == '\\') {
                printf("\\\\");
            } else {
                printf("%c", output_buffer[i]);
            }
        }
        printf("\"\n};\n\n");
        
        // Écrire la fonction generate_html
        printf("// Fonction pour générer un fichier HTML avec le contenu du buffer\n");
        printf("void generate_html(const char *filename) {\n");
        printf("    FILE *file = fopen(filename, \"w\");\n");
        printf("    if (file == NULL) {\n");
        printf("        fprintf(stderr, \"Error: Failed to open file %%s for writing\\n\", filename);\n");
        printf("        return;\n");
        printf("    }\n");
        printf("    \n");
        printf("    // Écrire l'en-tête HTML standard\n");
        printf("    fprintf(file, \"<!DOCTYPE html>\\n\");\n");
        printf("    fprintf(file, \"<html lang=\\\"en\\\">\\n\");\n");
        printf("    fprintf(file, \"<head>\\n\");\n");
        printf("    fprintf(file, \"    <meta charset=\\\"UTF-8\\\">\\n\");\n");
        printf("    fprintf(file, \"    <meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\");\n");
        printf("    fprintf(file, \"    <title>Generated Component</title>\\n\");\n");
        printf("    fprintf(file, \"</head>\\n\");\n");
        printf("    fprintf(file, \"<body>\\n\");\n");
        printf("    \n");
        printf("    // Écrire le contenu du buffer\n");
        printf("    fprintf(file, \"%%s\\n\", output_buffer);\n");
        printf("    \n");
        printf("    // Fermer le document HTML\n");
        printf("    fprintf(file, \"</body>\\n\");\n");
        printf("    fprintf(file, \"</html>\\n\");\n");
        printf("    \n");
        printf("    fclose(file);\n");
        printf("    printf(\"HTML file generated successfully: %%s\\n\", filename);\n");
        printf("}\n\n");
        
        // Ajouter une fonction main pour tester
        printf("int main(int argc, char *argv[]) {\n");
        // Déclarer les variables
        printf("// Déclaration des variables\n");
        declare_variables(); 
        printf("\n");
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 280 "parser.y"
    {
        // Create a buffer with sufficient space
        char buffer[10000] = {0};  // Initialize to zero
        
        // Reset the output buffer before processing this component
        output_buffer[0] = '\0';
        
        // Generate HTML component wrapper
        sprintf(buffer, "<div id='%s'>\n%s\n</div>", (yyvsp[(2) - (6)].strval), (yyvsp[(6) - (6)].strval));
        
        // Append to the global output buffer
        append_to_buffer(buffer);
        
        // Set the return value for this rule
        (yyval.strval) = strdup(buffer);
        
        // Free allocated memory
        free((yyvsp[(2) - (6)].strval));
        if ((yyvsp[(6) - (6)].strval)) free((yyvsp[(6) - (6)].strval));
    ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 300 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 304 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 305 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 306 "parser.y"
    {
        char *tmp = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 3);
        sprintf(tmp, "%s, %s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval));
        (yyval.strval) = tmp;
    ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 315 "parser.y"
    {
        char* tmp = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 3);
        add_identifier((yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        sprintf(tmp, "%s: %s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        (yyval.strval) = tmp;
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval));
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 325 "parser.y"
    {
        (yyval.strval) = (yyvsp[(2) - (3)].strval); // Simply pass the correctly formatted body up the parse tree
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 331 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 332 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // No need for additional processing, just pass up the instructions
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 338 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 339 "parser.y"
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

  case 15:

/* Line 1455 of yacc.c  */
#line 353 "parser.y"
    {
        // Générer une instruction de type
        (yyval.strval) = strdup(" "); // Store the type name
    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 357 "parser.y"
    {
        // Générer une instruction de variable
        (yyval.strval) = strdup(" "); // Store the variable name
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 361 "parser.y"
    {
        // Générer une instruction de retour
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // Store the return value
    ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 369 "parser.y"
    {add_custom_type((yyvsp[(2) - (3)].strval));;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 370 "parser.y"
    {
        // Générer une instruction de type
        (yyval.strval) = (yyvsp[(2) - (8)].strval); // Store the type name
        
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 378 "parser.y"
    {
        // Combine properties
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 385 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 389 "parser.y"
    { 
        // Propriété du type : <nom>: <type>
        // Return formatted property
        char *buffer = malloc(strlen((yyvsp[(1) - (4)].strval)) + strlen((yyvsp[(3) - (4)].strval)) + 10);
        sprintf(buffer, "%s: %s;\n", (yyvsp[(1) - (4)].strval), (yyvsp[(3) - (4)].strval));
        (yyval.strval) = buffer;
        add_field_to_type((yyvsp[(1) - (4)].strval), (yyvsp[(3) - (4)].strval));
        free((yyvsp[(1) - (4)].strval)); free((yyvsp[(3) - (4)].strval));
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 401 "parser.y"
    {
        (yyval.strval) = (yyvsp[(3) - (5)].strval); // Store the HTML content
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 406 "parser.y"
    {
        // Traitement terminé, résultat déjà stocké dans field_names et field_values
        (yyval.strval) = strdup(""); // Simplement pour éviter les erreurs de syntaxe
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 410 "parser.y"
    {
        // Cas d'un objet vide
        (yyval.strval) = strdup("");
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 417 "parser.y"
    {
        // Ajoute simplement une nouvelle paire field_name:value
    ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 420 "parser.y"
    {
        // Premier champ
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 426 "parser.y"
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

  case 29:

/* Line 1455 of yacc.c  */
#line 440 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_STRING;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 448 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_NUMBER;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 456 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_BOOLEAN;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 464 "parser.y"
    {
        if (value_count < MAX_VALUES) {
            strcpy(field_values[value_count], (yyvsp[(1) - (1)].strval));
            field_types[value_count] = TYPE_IDENTIFIER;
            value_count++;
        }
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 475 "parser.y"
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

  case 34:

/* Line 1455 of yacc.c  */
#line 607 "parser.y"
    {
        char* var_name = (yyvsp[(1) - (6)].strval);
        char* type_name = (yyvsp[(3) - (6)].strval);
        int is_valid = 1;
        
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

  case 35:

/* Line 1455 of yacc.c  */
#line 674 "parser.y"
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
            printf("Array value string: %s\n", array_value_str); // Debugging line
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

  case 36:

/* Line 1455 of yacc.c  */
#line 716 "parser.y"
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
            printf("Custom array value string: %s\n", array_value); // Debugging line
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

  case 37:

/* Line 1455 of yacc.c  */
#line 770 "parser.y"
    {
        array_values[0] = (yyvsp[(1) - (1)].strval);
        array_value_count = 1;
    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 774 "parser.y"
    {
        if (array_value_count < MAX_ARRAY_VALUES) {
            array_values[array_value_count++] = (yyvsp[(3) - (3)].strval);
        } else {
            yyerror("Too many array values");
            YYERROR;
        }
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 785 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 788 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 791 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 794 "parser.y"
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

  case 43:

/* Line 1455 of yacc.c  */
#line 815 "parser.y"
    {
        strcpy(custom_array_elements[0], (yyvsp[(1) - (1)].strval));
        custom_array_element_count = 1;
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 820 "parser.y"
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

  case 45:

/* Line 1455 of yacc.c  */
#line 834 "parser.y"
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

  case 46:

/* Line 1455 of yacc.c  */
#line 878 "parser.y"
    {
        // Vérifier si l'identifiant existe
        char* type = get_identifier_type((yyvsp[(1) - (1)].strval));
        if (check_variable_exists((yyvsp[(1) - (1)].strval)) == 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas défini.", (yyvsp[(1) - (1)].strval));
            yyerror(error_msg);
            YYERROR;
        }
        
        // Obtenir la valeur via get_value (gère simple & structuré)
        char* value = get_value((yyvsp[(1) - (1)].strval), NULL);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s", (yyvsp[(1) - (1)].strval));
            yyerror(error_msg);
            YYERROR;
        }
        
        (yyval.strval) = value;  // déjà dupliqué dans get_value
        
        free((yyvsp[(1) - (1)].strval)); // Libérer la chaîne d'origine
        
    ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 902 "parser.y"
    {
        // Vérifier si le champ existe dans la structure
        char* var_name = (yyvsp[(1) - (3)].strval);
        char* field_name = (yyvsp[(3) - (3)].strval);
        
        // Vérifier si la variable existe
        if (check_variable_exists(var_name) == 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas une variable définie.", var_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Obtenir la valeur via get_value (gère simple & structuré)
        char* value = get_value(var_name, field_name);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s.%s", var_name, field_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        (yyval.strval) = value;  // déjà dupliqué dans get_value
        
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval)); // Libérer les chaînes d'origine
    ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 928 "parser.y"
    {
        // Array element access
        char* array_name = (yyvsp[(1) - (4)].strval);
        char* index_str = (yyvsp[(3) - (4)].strval);
        
        // Check if the array exists
        if (!check_variable_exists(array_name)) {
            char error_msg[100];
            sprintf(error_msg, "Error: Undefined array '%s'", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if it's actually an array
        int is_array = 0;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, array_name) == 0) {
                is_array = variables[i].is_array;
                break;
            }
        }
        
        if (!is_array) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not an array", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        char var_name[100];
        sprintf(var_name, "%s[%s]", array_name, index_str);
        char* value = get_value(var_name, NULL);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s", var_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        (yyval.strval) = value;
        free(array_name);
        free(index_str);
    ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 972 "parser.y"
    {
        // Access to field in array element
        char* array_name = (yyvsp[(1) - (6)].strval);
        char* index_str = (yyvsp[(3) - (6)].strval);
        char* field_name = (yyvsp[(6) - (6)].strval);
        
        // Check if the array exists
        if (!check_variable_exists(array_name)) {
            char error_msg[100];
            sprintf(error_msg, "Error: Undefined array '%s'", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if it's actually an array
        int is_array = 0;
        char* type_name = NULL;
        for (int i = 0; i < var_count; i++) {
            if (strcmp(variables[i].name, array_name) == 0) {
                is_array = variables[i].is_array;
                type_name = variables[i].type;
                break;
            }
        }
        
        if (!is_array) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not an array", array_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if the array type is a custom type
        custom_type* type = find_custom_type(type_name);
        if (type == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Error: '%s' is not a custom type", type_name);
            yyerror(error_msg);
            YYERROR;
        }
        
        // Check if the field exists in the custom type
        int field_exists = 0;
        for (int i = 0; i < type->field_count; i++) {
            if (strcmp(type->fields[i][0], field_name) == 0) {
                field_exists = 1;
                break;
            }
        }
        
        if (!field_exists) {
            char error_msg[100];
            sprintf(error_msg, "Error: Field '%s' does not exist in type '%s'", field_name, type_name);
            yyerror(error_msg);
            YYERROR;
        }

        char var_name[100];
        sprintf(var_name, "%s[%s]", array_name, index_str);
        char* value = get_value(var_name, field_name);
        if (value == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : Impossible d'obtenir la valeur de %s.%s", var_name, field_name);
            yyerror(error_msg);
            YYERROR;
        }
        (yyval.strval) = value;
        free(array_name);
        free(index_str);
        free(field_name);
    ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1047 "parser.y"
    {
        // Combine import instructions
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1054 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1058 "parser.y"
    {
        // Appeler process_import pour analyser le fichier importé
        process_import((yyvsp[(2) - (5)].strval), (yyvsp[(4) - (5)].strval));
        
        // Retourner le nom du composant importé
        (yyval.strval) = (yyvsp[(2) - (5)].strval);
        free((yyvsp[(4) - (5)].strval)); // Libérer la chaîne du chemin du fichier
    ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1069 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1070 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); 
        free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1077 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1083 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1089 "parser.y"
    {
        // Vérifier que les balises ouvrantes et fermantes correspondent
        if (strcmp((yyvsp[(2) - (9)].strval), (yyvsp[(8) - (9)].strval)) != 0) {
            char error_msg[100];
            sprintf(error_msg, "Erreur: Les balises <%s> et </%s> ne correspondent pas", (yyvsp[(2) - (9)].strval), (yyvsp[(8) - (9)].strval));
            yyerror(error_msg);
            YYERROR;
        }
        
        size_t buffer_size = strlen((yyvsp[(2) - (9)].strval)) * 2 + strlen((yyvsp[(3) - (9)].strval)) + strlen((yyvsp[(5) - (9)].strval)) + 100;
        char *buffer = malloc(buffer_size);
        
        if (buffer == NULL) {
            yyerror("Erreur d'allocation mémoire");
            YYERROR;
        }
        
        // Vérifier si c'est un composant importé
        char *imported_content = find_imported_component((yyvsp[(2) - (9)].strval));
        if (imported_content != NULL && strlen(imported_content) > 0) {
            // C'est un composant importé, utiliser son contenu
            sprintf(buffer, "%s", imported_content);
            append_to_buffer(buffer);  // Ajouter au buffer de sortie global
        } else {
            // Élément HTML normal
            sprintf(buffer, "<%s", (yyvsp[(2) - (9)].strval));
            
            // Ajouter les attributs si présents
            if (strlen((yyvsp[(3) - (9)].strval)) > 0) {
                char *attr_buffer = malloc(strlen((yyvsp[(3) - (9)].strval)) + 1);
                strcpy(attr_buffer, (yyvsp[(3) - (9)].strval));
                char *attr_token = strtok(attr_buffer, ";");
                
                while (attr_token != NULL) {
                    char *attr_name = strtok(attr_token, "=");
                    char *attr_value = strtok(NULL, "=");
                    
                    if (attr_name && attr_value) {
                        // Enlève les espaces de l'attribut
                        while (*attr_name == ' ') attr_name++;
                        while (*attr_value == ' ') attr_value++;
                        
                        // Ajoute l'attribut au buffer dans la balise ouverte
                        sprintf(buffer + strlen(buffer), " %s=%s", attr_name, attr_value);
                    }
                    
                    attr_token = strtok(NULL, ";");
                }
                
                free(attr_buffer);
            }
            
            // Ajouter le contenu de l'élément HTML
            sprintf(buffer + strlen(buffer), ">%s</%s>", (yyvsp[(5) - (9)].strval), (yyvsp[(2) - (9)].strval));
        }
        
        (yyval.strval) = buffer;
        free((yyvsp[(2) - (9)].strval)); free((yyvsp[(3) - (9)].strval)); free((yyvsp[(5) - (9)].strval)); free((yyvsp[(8) - (9)].strval)); 
    ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1148 "parser.y"
    {   
        char *buffer = malloc(1000);
        
        // Vérifier si c'est un composant importé
        char *imported_content = find_imported_component((yyvsp[(2) - (5)].strval));
        if (imported_content != NULL && strlen(imported_content) > 0) {
            // C'est un composant importé, utiliser son contenu
            strcpy(buffer, imported_content);
            append_to_buffer(buffer);  // Important: ajouter au buffer de sortie global
        } else {
            // Élément HTML auto-fermant normal
            sprintf(buffer, "<%s", (yyvsp[(2) - (5)].strval));
            
            // Traiter les attributs comme ci-dessus
            if (strlen((yyvsp[(3) - (5)].strval)) > 0) {
                char *attr_buffer = malloc(strlen((yyvsp[(3) - (5)].strval)) + 1);
                strcpy(attr_buffer, (yyvsp[(3) - (5)].strval));
                char *attr_token = strtok(attr_buffer, ";");
                
                while (attr_token != NULL) {
                    char *attr_name = strtok(attr_token, "=");
                    char *attr_value = strtok(NULL, "=");
                    
                    if (attr_name && attr_value) {
                        // Enlève les espaces
                        while (*attr_name == ' ') attr_name++;
                        while (*attr_value == ' ') attr_value++;
                        
                        // Ajoute l'attribut au buffer dans la balise ouverte
                        sprintf(buffer + strlen(buffer), " %s=%s", attr_name, attr_value);
                    }
                    
                    attr_token = strtok(NULL, ";");
                }
                
                free(attr_buffer);
            }
            
            // Ajouter la balise auto-fermante
            sprintf(buffer + strlen(buffer), " />");
        }
        
        (yyval.strval) = buffer;
        free((yyvsp[(2) - (5)].strval)); free((yyvsp[(3) - (5)].strval));
    ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1193 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1199 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1200 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s;%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1209 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 5);
        sprintf(buffer, "%s=%s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval));
    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1218 "parser.y"
    {
        char* type = get_identifier_type((yyvsp[(1) - (1)].strval));
        if (type != NULL) {
            // C'est un identifiant connu avec un type
            (yyval.strval) = generate_html_code((yyvsp[(1) - (1)].strval));
        } else {
            // Vérifier si c'est un composant importé
            char* component_content = find_imported_component((yyvsp[(1) - (1)].strval));
            
            if (component_content != NULL) {
                // C'est un composant importé
                char *buffer = malloc(strlen(component_content) + 100);
                
                // Insérer le contenu du composant importé
                sprintf(buffer, component_content, (yyvsp[(1) - (1)].strval));
                
                (yyval.strval) = buffer;
            } else {
                // C'est un littéral de texte
                char* tmp = malloc(strlen((yyvsp[(1) - (1)].strval)) + 100);
                sprintf(tmp, "%s", (yyvsp[(1) - (1)].strval));
                (yyval.strval) = tmp;
            }
        }

        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1245 "parser.y"
    {
        (yyval.strval)= (yyvsp[(2) - (3)].strval);
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2983 "parser.tab.c"
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
#line 1250 "parser.y"


void yyerror(const char *s) {
    extern char *yytext;  // yytext donne le token actuel
    fprintf(stderr, "Erreur de syntaxe : %s\n", s);
    fprintf(stderr, "Problème avec le token: '%s'\n", yytext);
    exit(1);
}

int main() {
    yydebug = 1;
    output_buffer[0] = '\0';
    
    
    initialize_imported_components(); // Initialiser la liste des composants importés
    int result = yyparse();
    
    free_imported_components(); // Libérer la mémoire des composants importés
    
    return result;
}
