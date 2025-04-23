#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* data;
    size_t size;
    size_t capacity;
} DynamicBuffer;

extern DynamicBuffer import_buffer ;
extern DynamicBuffer interfaces_buffer;
extern DynamicBuffer output_buffer;

void buffer_init(DynamicBuffer* buffer);
void buffer_append(DynamicBuffer* buffer, const char* content);
void buffer_free(DynamicBuffer* buffer);
void initialize_buffers() ;
void cleanup_buffers();
void generate_html_header(FILE* file);
void generate_html_body(FILE* file, const DynamicBuffer* content) ;
void generate_html_file(const char* filename, const DynamicBuffer* content);
void generate_output_buffer_code() ;

#endif