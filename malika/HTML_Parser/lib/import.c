#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "import.h"
#include "identifier.h"
#include "variable.h"
#include "buffer.h"


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

// Fonction pour générer les includes
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


// Analyse des dépendances en fonction du contenu des identifiants et types
void analyze_dependencies(const DynamicBuffer* output_buffer) {
    // Dépendances essentielles pour le fonctionnement de base
    needs_stdio = 1;  // Pour printf
    needs_stdlib = 1; // Pour malloc/free
    needs_string = 1; // Pour manipulations de chaînes
    
    // Analyse des types pour détecter des dépendances spécifiques
    for (int i = 0; i < id_count; i++) {
        if (strcmp(variables[i].type, "float") == 0 || strcmp(variables[i].type, "double") == 0) {
            needs_math = 1;
        }
        else if (strcmp(variables[i].type, "complex") == 0) {
            needs_complex = 1;
        }
        else if (strcmp(variables[i].type, "bool") == 0) {
            needs_stdbool = 1;
        }
        else if (strcmp(variables[i].type, "wchar_t") == 0) {
            needs_wchar = 1;
        }
        else if (strstr(variables[i].type, "time") != NULL) {
            needs_time = 1;
        }
        else if (strstr(variables[i].type, "int") != NULL) {
            needs_limits = 1; // Pour les limites de int
        }
    }
    
    // Analyse du contenu du buffer pour détecter d'autres dépendances
    if (strstr(output_buffer->data, "isalpha") != NULL || 
        strstr(output_buffer->data, "isdigit") != NULL || 
        strstr(output_buffer->data, "tolower") != NULL) {
        needs_ctype = 1;
    }
    
    if (strstr(output_buffer->data, "malloc") != NULL || 
        strstr(output_buffer->data, "free") != NULL || 
        strstr(output_buffer->data, "exit") != NULL) {
        needs_stdlib = 1;
    }
    
    if (strstr(output_buffer->data, "sin") != NULL || 
        strstr(output_buffer->data, "cos") != NULL || 
        strstr(output_buffer->data, "sqrt") != NULL) {
        needs_math = 1;
    }
    
    if (strstr(output_buffer->data, "printf") != NULL || 
        strstr(output_buffer->data, "scanf") != NULL || 
        strstr(output_buffer->data, "fprintf") != NULL) {
        needs_stdio = 1;
    }
    
    if (strstr(output_buffer->data, "strcpy") != NULL || 
        strstr(output_buffer->data, "strlen") != NULL || 
        strstr(output_buffer->data, "strcat") != NULL) {
        needs_string = 1;
    }
    
    if (strstr(output_buffer->data, "assert") != NULL) {
        needs_assert = 1;
    }
    
    if (strstr(output_buffer->data, "errno") != NULL) {
        needs_errno = 1;
    }
    
    if (strstr(output_buffer->data, "setjmp") != NULL || 
        strstr(output_buffer->data, "longjmp") != NULL) {
        needs_setjmp = 1;
    }
    
    if (strstr(output_buffer->data, "signal") != NULL) {
        needs_signal = 1;
    }
    
    if (strstr(output_buffer->data, "open") != NULL || 
        strstr(output_buffer->data, "close") != NULL || 
        strstr(output_buffer->data, "read") != NULL || 
        strstr(output_buffer->data, "write") != NULL) {
        needs_unistd = 1;
    }
    
    if (strstr(output_buffer->data, "O_RDONLY") != NULL || 
        strstr(output_buffer->data, "O_WRONLY") != NULL || 
        strstr(output_buffer->data, "O_CREAT") != NULL) {
        needs_fcntl = 1;
    }
    
    if (strstr(output_buffer->data, "thrd_") != NULL || 
        strstr(output_buffer->data, "mtx_") != NULL || 
        strstr(output_buffer->data, "cnd_") != NULL) {
        needs_threads = 1;
    }
}
