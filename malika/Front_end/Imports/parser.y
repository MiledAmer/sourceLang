%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YYDEBUG 1
extern int yylex();
void yyerror(const char *s);

// Buffer to store parsed code
char output_buffer[10000];  
int buffer_index = 0;

// Track required headers
int needs_assert = 0;
int needs_complex = 0;
int needs_ctype = 0;
int needs_errno = 0;
int needs_fenv = 0;
int needs_float = 0;
int needs_inttypes = 0;
int needs_limits = 0;
int needs_locale = 0;
int needs_math = 0;
int needs_setjmp = 0;
int needs_signal = 0;
int needs_stdio = 0;
int needs_stdlib = 0;
int needs_string = 0;
int needs_time = 0;
int needs_wchar = 0;
int needs_wctype = 0;
int needs_tgmath = 0;
int needs_stddef = 0;
int needs_stdbool = 0;
int needs_stdarg = 0;
int needs_stdalign = 0;
int needs_iso646 = 0;
int needs_unistd = 0;
int needs_fcntl = 0;
int needs_threads = 0;

// Function to store output in buffer
void append_to_buffer(const char *text) {
    snprintf(output_buffer + buffer_index, sizeof(output_buffer) - buffer_index, "%s", text);
    buffer_index += strlen(text);
}

// Function to generate includes
void generate_includes() {
    printf("/* Includes automatiques */\n");

    if (needs_assert) printf("#include <assert.h>\n");
    if (needs_complex) printf("#include <complex.h>\n");
    if (needs_ctype) printf("#include <ctype.h>\n");
    if (needs_errno) printf("#include <errno.h>\n");
    if (needs_fenv) printf("#include <fenv.h>\n");
    if (needs_float) printf("#include <float.h>\n");
    if (needs_inttypes) printf("#include <inttypes.h>\n");
    if (needs_limits) printf("#include <limits.h>\n");
    if (needs_locale) printf("#include <locale.h>\n");
    if (needs_math) printf("#include <math.h>\n");
    if (needs_setjmp) printf("#include <setjmp.h>\n");
    if (needs_signal) printf("#include <signal.h>\n");
    if (needs_stdio) printf("#include <stdio.h>\n");
    if (needs_stdlib) printf("#include <stdlib.h>\n");
    if (needs_string) printf("#include <string.h>\n");
    if (needs_threads) printf("#include <threads.h>\n");
    if (needs_time) printf("#include <time.h>\n");
    if (needs_wchar) printf("#include <wchar.h>\n");
    if (needs_wctype) printf("#include <wctype.h>\n");
    if (needs_tgmath) printf("#include <tgmath.h>\n");
    if (needs_stddef) printf("#include <stddef.h>\n");
    if (needs_stdbool) printf("#include <stdbool.h>\n");
    if (needs_stdarg) printf("#include <stdarg.h>\n");
    if (needs_stdalign) printf("#include <stdalign.h>\n");
    if (needs_iso646) printf("#include <iso646.h>\n");


    printf("\n"); // Space between includes and code
}
%}

%union {
    char* strval;
}

// Tokens
%token <strval> ASSERT COMPLEX CTYPE ERRNO FENV FCNTL FLOAT INTTYPES LIMITS LOCALE MATH SETJMP SIGNAL STDIO STDLIB STRING THREADS TIME WCHAR WCTYPE TGMATH STDDEF STDBOOL STDARG STDALIGN ISO646 IDENTIFIER UNISTD 
%type <strval> function program 
 
%start program

%%

// Program stores parsed content and prints at the end
program:
    function { 
        generate_includes(); 
        printf("%s", output_buffer); // Print stored code
    }
    ;

function:
    STRING { needs_string = 1; append_to_buffer($1); free($1); }
    | MATH { needs_math = 1; append_to_buffer($1); free($1); }
    | STDLIB { needs_stdlib = 1; append_to_buffer($1); free($1); }
    | STDIO { needs_stdio = 1; append_to_buffer($1); free($1); }
    | CTYPE { needs_ctype = 1; append_to_buffer($1); free($1); }
    | TIME { needs_time = 1; append_to_buffer($1); free($1); }
    | UNISTD { needs_unistd = 1; append_to_buffer($1); free($1); }
    | ASSERT { needs_assert = 1; append_to_buffer($1); free($1); }
    | FCNTL { needs_fcntl = 1; append_to_buffer($1); free($1); }
    | ERRNO { needs_errno = 1; append_to_buffer($1); free($1); }
    | SIGNAL { needs_signal = 1; append_to_buffer($1); free($1); }
    | COMPLEX { needs_complex = 1; append_to_buffer($1); free($1); }
    | FENV { needs_fenv = 1; append_to_buffer($1); free($1); }
    | FLOAT { needs_float = 1; append_to_buffer($1); free($1); }
    | INTTYPES { needs_inttypes = 1; append_to_buffer($1); free($1); }
    | LIMITS { needs_limits = 1; append_to_buffer($1); free($1); }
    | LOCALE { needs_locale = 1; append_to_buffer($1); free($1); }
    | SETJMP { needs_setjmp = 1; append_to_buffer($1); free($1); }
    | THREADS { needs_threads = 1; append_to_buffer($1); free($1); }
    | WCHAR { needs_wchar = 1; append_to_buffer($1); free($1); }
    | WCTYPE { needs_wctype = 1; append_to_buffer($1); free($1); }
    | TGMATH { needs_tgmath = 1; append_to_buffer($1); free($1); }
    | STDDEF { needs_stddef = 1; append_to_buffer($1); free($1); }
    | STDBOOL { needs_stdbool = 1; append_to_buffer($1); free($1); }
    | STDARG { needs_stdarg = 1; append_to_buffer($1); free($1); }
    | STDALIGN { needs_stdalign = 1; append_to_buffer($1); free($1); }
    | ISO646 { needs_iso646 = 1; append_to_buffer($1); free($1); }
    | IDENTIFIER { append_to_buffer($1); free($1); }
    | function function  // Allows multiple functions
    ;

%%

int main() {
    yydebug = 1;
    if (yyparse() == 0) {
        return 0;
    } else {
        printf("Parsing failed\n");
        return 1;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
