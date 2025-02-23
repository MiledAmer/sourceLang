%{
#include "parser.tab.h"  // Include parser declarations
#include <string.h>
#include <stdlib.h>

%}

%%


"assert" { yylval.strval = strdup(yytext); return ASSERT; }


"cabs"|"carg"|"cimag"|"creal" { yylval.strval = strdup(yytext); return COMPLEX; }


"isalnum"|"isalpha"|"isdigit"|"islower"|"isupper"|"isxdigit"|"iscntrl"|"isgraph"|"isprint"|"ispunct"|"isspace"|"tolower"|"toupper" {  yylval.strval = strdup(yytext); return CTYPE; }


"errno" { yylval.strval = strdup(yytext); return ERRNO; }


"feclearexcept"|"feraiseexcept"|"fetestexcept"|"fegetround"|"fesetround" { yylval.strval = strdup(yytext); return FENV; }

"open"|"creat"|"fcntl"|"close" { yylval.strval = strdup(yytext); return FCNTL; }

"FLT_MIN"|"FLT_MAX"|"DBL_MIN"|"DBL_MAX" { yylval.strval = strdup(yytext); return FLOAT; }

"and"|"or"|"xor"|"not"|"bitand"|"bitor"|"compl"|"and_eq"|"or_eq"|"xor_eq"|"not_eq" { yylval.strval = strdup(yytext); return ISO646; }

"PRId32"|"PRIu64"|"SCNd8"|"SCNx16" { yylval.strval = strdup(yytext); return INTTYPES; }

"CHAR_BIT"|"SCHAR_MIN"|"SCHAR_MAX"|"UCHAR_MAX"|"SHRT_MIN"|"SHRT_MAX"|"USHRT_MAX" { yylval.strval = strdup(yytext); return LIMITS; }

"setlocale"|"localeconv" { yylval.strval = strdup(yytext); return LOCALE; }

"sin"|"cos"|"tan"|"asin"|"acos"|"atan"|"atan2"|"sinh"|"cosh"|"tanh"|"exp"|"log"|"log10"|"pow"|"sqrt"|"ceil"|"floor"|"fabs" { yylval.strval = strdup(yytext); return MATH; }

"NULL"|"size_t"|"ptrdiff_t"|"offsetof" { yylval.strval = strdup(yytext); return STDDEF; }

"true"|"false"|"bool" { yylval.strval = strdup(yytext); return STDBOOL; }

"va_start"|"va_arg"|"va_copy"|"va_end" { yylval.strval = strdup(yytext); return STDARG; }

"alignas"|"alignof" { yylval.strval = strdup(yytext); return STDALIGN; }

"setjmp"|"longjmp" { yylval.strval = strdup(yytext); return SETJMP; }

"signal"|"raise" { yylval.strval = strdup(yytext); return SIGNAL; }

"printf"|"scanf"|"fprintf"|"fscanf"|"sprintf"|"sscanf"|"vprintf"|"vfprintf"|"vsprintf"|"vscanf"|"vfscanf"|"vsscanf"|"fopen"|"fclose"|"fread"|"fwrite"|"fseek"|"ftell"|"rewind"|"fflush" { yylval.strval = strdup(yytext); return STDIO; }

"malloc"|"calloc"|"realloc"|"free"|"atoi"|"atol"|"atof"|"strtol"|"strtoul"|"strtod"|"abs"|"labs"|"rand"|"srand" { yylval.strval = strdup(yytext); return STDLIB; }

"strlen"|"strcpy"|"strcat"|"strcmp"|"strncpy"|"strncat"|"strchr"|"strrchr"|"strstr" { yylval.strval = strdup(yytext); return STRING; }

"tgamma"|"lgamma"|"hypot" { yylval.strval = strdup(yytext); return TGMATH; }

"pthread_create"|"pthread_join"|"pthread_mutex_lock"|"pthread_mutex_unlock" { yylval.strval = strdup(yytext); return THREADS; }

"time"|"clock"|"difftime"|"mktime"|"strftime"|"gmtime"|"localtime" { yylval.strval = strdup(yytext); return TIME; }

"wcslen"|"wcscpy"|"wcscat"|"wcschr"|"wcsrchr" { yylval.strval = strdup(yytext); return WCHAR; }

"iswalpha"|"iswdigit"|"iswlower"|"iswupper"|"towupper"|"towlower" { yylval.strval = strdup(yytext); return WCTYPE; }

.|[ \t\n]+                      { yylval.strval = strdup(yytext); return IDENTIFIER; }

%%

int yywrap() {
    return 1;
}
