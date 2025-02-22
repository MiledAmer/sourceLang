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
int needs_string = 0;
int needs_math = 0;
int needs_stdlib = 0;
int needs_stdio = 0;
int needs_ctype = 0;
int needs_time = 0;
int needs_unistd = 0;
int needs_assert = 0;
int needs_fcntl = 0;
int needs_pthread = 0;
int needs_errno = 0;
int needs_signal = 0;

// Function to store output in buffer
void append_to_buffer(const char *text) {
    snprintf(output_buffer + buffer_index, sizeof(output_buffer) - buffer_index, "%s", text);
    buffer_index += strlen(text);
}

// Function to generate includes
void generate_includes() {
    printf("/* Includes automatiques */\n");

    if (needs_stdio) printf("#include <stdio.h>\n");
    if (needs_string) printf("#include <string.h>\n");
    if (needs_math) printf("#include <math.h>\n");
    if (needs_stdlib) printf("#include <stdlib.h>\n");
    if (needs_ctype) printf("#include <ctype.h>\n");
    if (needs_time) printf("#include <time.h>\n");
    if (needs_unistd) printf("#include <unistd.h>\n");
    if (needs_assert) printf("#include <assert.h>\n");
    if (needs_fcntl) printf("#include <fcntl.h>\n");
    if (needs_pthread) printf("#include <pthread.h>\n");
    if (needs_errno) printf("#include <errno.h>\n");
    if (needs_signal) printf("#include <signal.h>\n");

    printf("\n"); // Space between includes and code
}
%}

%union {
    char* strval;
}

// Tokens
%token STRING MATH STDLIB STDIO CTYPE TIME UNISTD ASSERT FCNTL PTHREAD ERRNO SIGNAL IDENTIFIER
%type <strval> function program STRING MATH STDLIB STDIO CTYPE TIME UNISTD ASSERT FCNTL PTHREAD ERRNO SIGNAL IDENTIFIER
 
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
    STRING { needs_string = 1; append_to_buffer($1);  free($1); }
    | MATH { needs_math = 1; append_to_buffer($1);  free($1); }
    | STDLIB { needs_stdlib = 1; append_to_buffer($1);  free($1); }
    | STDIO { needs_stdio = 1; append_to_buffer($1); free($1); }
    | CTYPE { needs_ctype = 1; append_to_buffer($1); free($1); }
    | TIME { needs_time = 1; append_to_buffer($1); free($1); }
    | UNISTD { needs_unistd = 1; append_to_buffer($1);  free($1); }
    | ASSERT { needs_assert = 1; append_to_buffer($1);  free($1); }
    | FCNTL { needs_fcntl = 1; append_to_buffer($1);  free($1); }
    | PTHREAD { needs_pthread = 1; append_to_buffer($1);  free($1); }
    | ERRNO { needs_errno = 1; append_to_buffer($1); free($1); }
    | SIGNAL { needs_signal = 1; append_to_buffer($1);  free($1); }
    | IDENTIFIER { append_to_buffer($1);  free($1); }
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
