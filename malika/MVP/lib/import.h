#ifndef IMPORT_H
#define IMPORT_H
#include "buffer.h"

extern int needs_assert ;
extern int needs_complex ;
extern int needs_ctype ;
extern int needs_errno ;
extern int needs_fenv ;
extern int needs_float ;
extern int needs_inttypes ;
extern int needs_limits ;
extern int needs_locale ;
extern int needs_math ;
extern int needs_setjmp ;
extern int needs_signal ;
extern int needs_stdio ;
extern int needs_stdlib ;
extern int needs_string ;
extern int needs_time ;
extern int needs_wchar ;
extern int needs_wctype ;
extern int needs_tgmath ;
extern int needs_stddef ;
extern int needs_stdbool ;
extern int needs_stdarg ;
extern int needs_stdalign ;
extern int needs_iso646 ;
extern int needs_unistd;
extern int needs_fcntl ;
extern int needs_threads;

// Fonction pour générer les includes
void generate_includes();

// Analyse des dépendances en fonction du contenu des identifiants et types
void analyze_dependencies(const DynamicBuffer* output_buffer); ;

#endif