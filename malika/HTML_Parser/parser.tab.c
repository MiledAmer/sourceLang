
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

extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;  
extern void yyrestart(FILE* input_file);
#define YYDEBUG 1

char output_buffer[10000];
char interfaces_buffer[1000];  // Buffer pour les interfaces TypeScript
int id_count = 0;
int found = 0;

// Structure pour stocker les composants importés
typedef struct {
    char* name;
    char* html_content;
} ImportedComponent;

#define MAX_IMPORTED_COMPONENTS 50
ImportedComponent imported_components[MAX_IMPORTED_COMPONENTS];
int imported_count = 0;

// Structure pour les identifiants typés
typedef struct {
    char* name;
    char* type;
} TypedIdentifier;

TypedIdentifier identifiers[100];
char current_component_name[100] = ""; // Nom du composant en cours d'analyse

typedef struct {
    FILE* yyin_backup;
    char output_buffer_backup[10000];
    char current_component_name_backup[100];
    TypedIdentifier identifiers_backup[100];
    int id_count_backup;
} ParserState;

// Structure pour les types personnalisés
typedef struct {
    char name[50];
    char fields[10][2][50]; // [nombre_de_champs][nom_ou_type][chaine]
    int field_count;
} custom_type;

custom_type custom_types[20]; // Tableau pour stocker les types personnalisés
int type_count = 0;

// Structure pour stocker les variables
typedef struct {
    char name[50];
    char type[50];
    char value[500];
    int is_array;
} variable;

variable variables[100]; // Tableau pour stocker les variables
int var_count = 0;


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

void add_custom_type(char* name) {
    strcpy(custom_types[type_count].name, name);
    custom_types[type_count].field_count = 0;
    type_count++;
}

void add_field_to_type(char* field_name, char* field_type) {
    printf("DEBUG: Ajout du champ %s de type %s au type_count = %d\n", 
           field_name, field_type, type_count-1);
    fflush(stdout);
    
    if (type_count <= 0) {
        printf("ERREUR: Tentative d'ajout d'un champ sans type défini\n");
        fflush(stdout);
        return;
    }
    
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][0], field_name);
    strcpy(custom_types[type_count-1].fields[custom_types[type_count-1].field_count][1], field_type);
    custom_types[type_count-1].field_count++;
}

void generate_structs_and_prototypes() {
    for (int i = 0; i < type_count; i++) {
        printf("typedef struct {\n");
        fflush(stdout);
        
        for (int j = 0; j < custom_types[i].field_count; j++) {
            char* field_type = custom_types[i].fields[j][1];
            if (strcmp(field_type, "string") == 0) {
                printf("    char* ");
            } else {
                printf("    ");
                printf(field_type);
                printf(" ");
            }
            printf(custom_types[i].fields[j][0]);
            printf(";\n");
            fflush(stdout);
        }
        
        printf("} ");
        printf(custom_types[i].name);
        printf(";\n");
        fflush(stdout);
    }
}

// Fonction pour ajouter une variable
void add_variable(char* name, char* type, char* value, int is_array) {
    strcpy(variables[var_count].name, name);
    strcpy(variables[var_count].type, type);
    strcpy(variables[var_count].value, value);
    variables[var_count].is_array = is_array;
    var_count++;
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

// Fonction pour ajouter un identifiant et son type
void add_identifier(const char* name, const char* type) {
    identifiers[id_count].name = strdup(name);
    identifiers[id_count].type = strdup(type);
    id_count++;
}

// Fonction pour obtenir le type d'un identifiant
char* get_identifier_type(const char* name) {
    for (int i = 0; i < id_count; i++) {
        if (strcmp(identifiers[i].name, name) == 0) {
            return identifiers[i].type;
        }
    }
    return NULL;
}

// Fonction pour générer le code HTML pur selon le type
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

// Fonction pour trouver un composant importé par son nom
char* find_imported_component(const char* name) {
    // printf("Recherche du composant importé: %s\n", name);
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // printf("Composant trouvé: %s\n", name);
            // printf("Contenu: %s\n", imported_components[i].html_content);
            return imported_components[i].html_content;
        }
    }
    return NULL;
}

// Fonction pour ajouter un composant importé
void add_imported_component(const char* name, const char* html_content) {
    // Vérifier si le composant existe déjà
    for (int i = 0; i < imported_count; i++) {
        if (strcmp(imported_components[i].name, name) == 0) {
            // Remplacer le contenu existant
            free(imported_components[i].html_content);
            imported_components[i].html_content = strdup(html_content);
            return;
        }
    }
    
    // Ajouter un nouveau composant
    if (imported_count < MAX_IMPORTED_COMPONENTS) {
        imported_components[imported_count].name = strdup(name);
        imported_components[imported_count].html_content = strdup(html_content);
        imported_count++;
    } else {
        fprintf(stderr, "Error: Maximum number of imported components reached\n");
    }
}
void process_import(char* component, char* path) {
    // Enlever les guillemets du chemin
    char real_path[256];
    strncpy(real_path, path + 1, strlen(path) - 2);  // Remove beginning and ending quotes
    real_path[strlen(path) - 2] = '\0';
    
    // Vérifier si le chemin est relatif et ajouter le ./ si nécessaire
    if (real_path[0] != '/' && strncmp(real_path, "./", 2) != 0) {
        char temp[256];
        sprintf(temp, "./%s", real_path);
        strcpy(real_path, temp);
    }
    
    // printf("Traitement de l'import du composant %s depuis %s\n", component, real_path);
    
    // Ouvrir le fichier source
    FILE* imported_file = fopen(real_path, "r");
    if (!imported_file) {
        fprintf(stderr, "Erreur d'ouverture du fichier %s\n", real_path);
        exit(1);
    }
    
    // Lire le contenu du fichier
    char buffer[10000] = {0};
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, imported_file);
    buffer[bytes_read] = '\0';  // Assurer que la chaîne est terminée par null
    
    // printf("Contenu brut du fichier importé:\n%s\n", buffer);
    
    // Analyser manuellement le contenu pour extraire le HTML du composant
    // Ceci est une version simplifiée qui suppose que le composant est correctement formaté
    char* start = strstr(buffer, "component");
    if (start) {
        start = strstr(start, "{");
        if (start) {
            char* return_stmt = strstr(start, "return");
            if (return_stmt) {
                char* open_paren = strstr(return_stmt, "(");
                if (open_paren) {
                    char* close_paren = strrchr(buffer, ')');  // Trouver la dernière parenthèse fermante
                    if (close_paren) {
                        // Extraire le contenu HTML entre les parenthèses de l'instruction return
                        int html_length = close_paren - (open_paren + 1);
                        if (html_length > 0 && html_length < 9000) {
                            char html_content[10000] = {0};
                            strncpy(html_content, open_paren + 1, html_length);
                            html_content[html_length] = '\0';
                            
                            // Créer un div englobant avec le nom du composant comme classe
                            char final_html[10000] = {0};
                            sprintf(final_html, "<div id='%s'>\n%s\n</div>", component, html_content);
                            
                            // Ajouter le composant importé à notre registre
                            add_imported_component(component, final_html);
                            // printf("Composant importé %s ajouté avec le contenu:\n%s\n", component, final_html);
                        }
                    }
                }
            }
        }
    }
    
    // Si l'analyse manuelle n'a pas fonctionné, créer un composant minimal
    if (find_imported_component(component) == NULL) {
        char minimal_html[1000];
        sprintf(minimal_html, "<div id='%s'><!-- Contenu du composant %s non analysé --></div>", 
                component, component);
        add_imported_component(component, minimal_html);
        // printf("Impossible d'analyser le composant %s. Utilisation d'un composant minimal.\n", component);
    }
    
    // Fermer le fichier
    fclose(imported_file);
    
    // printf("Import du composant %s terminé\n", component);
}



/* Line 189 of yacc.c  */
#line 400 "parser.tab.c"

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
     IDENTIFIER = 274,
     STRING_LITERAL = 275
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 327 "parser.y"

    int intval;
    char* strval;



/* Line 214 of yacc.c  */
#line 463 "parser.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 475 "parser.tab.c"

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
#define YYLAST   77

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  21
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  38
/* YYNRULES -- Number of states.  */
#define YYNSTATES  76

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   275

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
      15,    16,    17,    18,    19,    20
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    15,    16,    17,    19,    23,
      27,    31,    32,    34,    36,    39,    41,    43,    44,    53,
      56,    58,    63,    66,    68,    74,    80,    81,    84,    87,
      89,    99,   105,   107,   109,   110,   113,   117,   119
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      22,     0,    -1,    23,    -1,    34,    23,    -1,     3,    19,
       8,    24,     9,    26,    -1,    -1,    -1,    25,    -1,    25,
      15,    24,    -1,    19,    10,    19,    -1,     6,    27,     7,
      -1,    -1,    28,    -1,    29,    -1,    29,    28,    -1,    30,
      -1,    36,    -1,    -1,    11,    19,     5,    31,     6,    32,
       7,    12,    -1,    32,    33,    -1,    33,    -1,    19,    10,
      19,    12,    -1,    34,    35,    -1,    35,    -1,    14,    19,
      13,    20,    12,    -1,     4,     8,    37,     9,    12,    -1,
      -1,    37,    38,    -1,    37,    41,    -1,    38,    -1,    16,
      19,    39,    17,    37,    16,    18,    19,    17,    -1,    16,
      19,    39,    18,    17,    -1,    41,    -1,    19,    -1,    -1,
      40,    39,    -1,    19,     5,    20,    -1,    19,    -1,     6,
      19,     7,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   341,   341,   413,   486,   506,   510,   511,   512,   521,
     531,   537,   538,   544,   545,   554,   558,   565,   565,   574,
     581,   585,   597,   604,   608,   619,   625,   626,   633,   639,
     645,   704,   749,   752,   770,   771,   780,   789,   816
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMPONENT", "RETURN", "EQUALS",
  "LBRACE", "RBRACE", "LPAREN", "RPAREN", "COLON", "TYPE", "SEMICOLON",
  "FROM", "IMPORT", "COMMA", "LT", "GT", "SLASH", "IDENTIFIER",
  "STRING_LITERAL", "$accept", "program", "element", "parameters",
  "parameter", "function", "function_body", "instructions", "instruction",
  "type_instruction", "$@1", "type_properties", "type_property",
  "import_instructions", "import_instruction", "return_instruction",
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
     275
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    21,    22,    22,    23,    23,    24,    24,    24,    25,
      26,    27,    27,    28,    28,    29,    29,    31,    30,    32,
      32,    33,    34,    34,    35,    36,    37,    37,    37,    37,
      38,    38,    38,    38,    39,    39,    40,    41,    41
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     6,     0,     0,     1,     3,     3,
       3,     0,     1,     1,     2,     1,     1,     0,     8,     2,
       1,     4,     2,     1,     5,     5,     0,     2,     2,     1,
       9,     5,     1,     1,     0,     2,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     0,     0,     2,     5,    23,     0,     0,     1,
       3,    22,     6,     0,     0,     0,     7,     0,     0,     0,
       6,    24,     9,    11,     4,     8,     0,     0,     0,    12,
      13,    15,    16,    26,     0,    10,    14,     0,     0,    33,
       0,    29,    32,    17,     0,    34,     0,    27,    28,     0,
      38,     0,     0,    34,    25,     0,     0,     0,     0,    35,
       0,     0,    20,    36,     0,    31,     0,     0,    19,     0,
       0,    18,     0,    21,     0,    30
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,    15,    16,    24,    28,    29,    30,    31,
      49,    61,    62,     5,     6,    32,    40,    41,    52,    53,
      42
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -41
static const yytype_int8 yypact[] =
{
       5,   -12,     4,    11,   -41,     5,   -41,    18,    15,   -41,
     -41,   -41,    10,    12,    20,    22,    19,    21,    17,    29,
      10,   -41,   -41,    16,   -41,   -41,    30,    23,    32,   -41,
      16,   -41,   -41,    -3,    35,   -41,   -41,    24,    25,   -41,
      -4,   -41,   -41,   -41,    34,    26,    36,   -41,   -41,    31,
     -41,    41,    -8,    26,   -41,    28,    33,    -3,    37,   -41,
      39,    -1,   -41,   -41,    -2,   -41,    38,    40,   -41,     3,
      43,   -41,    42,   -41,    45,   -41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,    46,    44,   -41,   -41,   -41,    47,   -41,   -41,
     -41,   -41,   -11,   -41,    51,   -41,     1,   -40,     6,   -41,
     -39
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      47,    48,    37,    37,    37,    46,    67,     7,     1,    57,
      58,     9,    38,    38,    69,    39,    39,    39,    60,     2,
      26,    72,    45,     8,    47,    48,    12,    27,    13,    14,
      18,    19,    17,    21,    20,    23,    22,    55,    33,    35,
      43,    50,    34,    44,    45,    51,    56,    60,    54,    66,
      68,    10,    71,    63,    65,    73,    11,    70,    64,    59,
       0,    74,    75,     0,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    36
};

static const yytype_int8 yycheck[] =
{
      40,    40,     6,     6,     6,     9,     7,    19,     3,    17,
      18,     0,    16,    16,    16,    19,    19,    19,    19,    14,
       4,    18,    19,    19,    64,    64,     8,    11,    13,    19,
      10,     9,    20,    12,    15,     6,    19,     6,     8,     7,
       5,     7,    19,    19,    19,    19,     5,    19,    12,    10,
      61,     5,    12,    20,    17,    12,     5,    19,    57,    53,
      -1,    19,    17,    -1,    20,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    14,    22,    23,    34,    35,    19,    19,     0,
      23,    35,     8,    13,    19,    24,    25,    20,    10,     9,
      15,    12,    19,     6,    26,    24,     4,    11,    27,    28,
      29,    30,    36,     8,    19,     7,    28,     6,    16,    19,
      37,    38,    41,     5,    19,    19,     9,    38,    41,    31,
       7,    19,    39,    40,    12,     6,     5,    17,    18,    39,
      19,    32,    33,    20,    37,    17,    10,     7,    33,    16,
      19,    12,    18,    12,    19,    17
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
#line 341 "parser.y"
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
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 413 "parser.y"
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
        printf("    const char *output_file = (argc > 1) ? argv[1] : \"output.html\";\n");
        printf("    generate_html(output_file);\n");
        printf("    return 0;\n");
        printf("}\n");
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 486 "parser.y"
    {
        // Create a buffer with sufficient space
        char buffer[10000] = {0};  // Initialize to zero
        
        // Reset the output buffer before processing this component
        output_buffer[0] = '\0';
        
        // Generate HTML component wrapper
        sprintf(buffer, "<div class='%s'>\n%s\n</div>", (yyvsp[(2) - (6)].strval), (yyvsp[(6) - (6)].strval));
        
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
#line 506 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 510 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 511 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 512 "parser.y"
    {
        char *tmp = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 3);
        sprintf(tmp, "%s, %s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval));
        (yyval.strval) = tmp;
    ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 521 "parser.y"
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
#line 531 "parser.y"
    {
        (yyval.strval) = (yyvsp[(2) - (3)].strval); // Simply pass the correctly formatted body up the parse tree
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 537 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 538 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // No need for additional processing, just pass up the instructions
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 544 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 545 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s\n%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 554 "parser.y"
    {
        // Générer une instruction de type
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // Store the type name
    ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 558 "parser.y"
    {
        // Générer une instruction de retour
        (yyval.strval) = (yyvsp[(1) - (1)].strval); // Store the return value
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 565 "parser.y"
    {add_custom_type((yyvsp[(2) - (3)].strval));;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 566 "parser.y"
    {
        // Générer une instruction de type
        (yyval.strval) = (yyvsp[(2) - (8)].strval); // Store the type name
        
    ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 574 "parser.y"
    {
        // Combine properties
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 581 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 585 "parser.y"
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

  case 22:

/* Line 1455 of yacc.c  */
#line 597 "parser.y"
    {
        // Combine import instructions
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 604 "parser.y"
    { (yyval.strval) = (yyvsp[(1) - (1)].strval); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 608 "parser.y"
    {
        // Appeler process_import pour analyser le fichier importé
        process_import((yyvsp[(2) - (5)].strval), (yyvsp[(4) - (5)].strval));
        
        // Retourner le nom du composant importé
        (yyval.strval) = (yyvsp[(2) - (5)].strval);
        free((yyvsp[(4) - (5)].strval)); // Libérer la chaîne du chemin du fichier
    ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 619 "parser.y"
    {
        (yyval.strval) = (yyvsp[(3) - (5)].strval); // Store the HTML content
    ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 625 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 626 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); 
        free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 633 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 639 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 645 "parser.y"
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

  case 31:

/* Line 1455 of yacc.c  */
#line 704 "parser.y"
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

  case 32:

/* Line 1455 of yacc.c  */
#line 749 "parser.y"
    {
        (yyval.strval) = (yyvsp[(1) - (1)].strval);
    ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 752 "parser.y"
    {
        // Vérifier si c'est un composant importé
        char* component_content = find_imported_component((yyvsp[(1) - (1)].strval));
        
        if (component_content != NULL && strlen(component_content) > 0) {
            // C'est un composant importé
            char *buffer = malloc(strlen(component_content) + 1);
            strcpy(buffer, component_content);
            (yyval.strval) = buffer;
        } else {
            // Ce n'est pas un composant importé, traiter comme un littéral de texte
            (yyval.strval) = strdup((yyvsp[(1) - (1)].strval));
        }
        
        free((yyvsp[(1) - (1)].strval));
    ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 770 "parser.y"
    { (yyval.strval) = strdup(""); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 771 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (2)].strval)) + strlen((yyvsp[(2) - (2)].strval)) + 2);
        sprintf(buffer, "%s;%s", (yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (2)].strval)); free((yyvsp[(2) - (2)].strval));
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 780 "parser.y"
    {
        char *buffer = malloc(strlen((yyvsp[(1) - (3)].strval)) + strlen((yyvsp[(3) - (3)].strval)) + 5);
        sprintf(buffer, "%s=%s", (yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval));
        (yyval.strval) = buffer;
        free((yyvsp[(1) - (3)].strval)); free((yyvsp[(3) - (3)].strval));
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 789 "parser.y"
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

  case 38:

/* Line 1455 of yacc.c  */
#line 816 "parser.y"
    {
        char* type = get_identifier_type((yyvsp[(2) - (3)].strval));
        if (type == NULL) {
            char error_msg[100];
            sprintf(error_msg, "Erreur : L'identifiant %s n'est pas un paramètre.", (yyvsp[(2) - (3)].strval));
            append_to_buffer(error_msg);
            yyerror(error_msg);
            YYERROR;
        } else {
            (yyval.strval) = generate_html_code((yyvsp[(2) - (3)].strval));
        }

        free((yyvsp[(2) - (3)].strval));
    ;}
    break;



/* Line 1455 of yacc.c  */
#line 2383 "parser.tab.c"
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
#line 832 "parser.y"


void yyerror(const char *s) {
    extern char *yytext;  // yytext donne le token actuel
    fprintf(stderr, "Erreur de syntaxe : %s\n", s);
    fprintf(stderr, "Problème avec le token: '%s'\n", yytext);
    exit(1);
}

int main() {
    yydebug = 1;
    output_buffer[0] = '\0';
    
    // Initialiser le stockage des composants importés
    for (int i = 0; i < MAX_IMPORTED_COMPONENTS; i++) {
        imported_components[i].name = NULL;
        imported_components[i].html_content = NULL;
    }
    
    int result = yyparse();
    
    // Libérer la mémoire des composants importés
    for (int i = 0; i < imported_count; i++) {
        free(imported_components[i].name);
        free(imported_components[i].html_content);
    }
    
    return result;
}
