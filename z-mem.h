#pragma once

#include <stddef.h>

void* mem_alloc(size_t len);
void* mem_zalloc(size_t len);
void  mem_free(void* p);
void* mem_realloc(void* p, size_t len);

char* mem_strdup(const char* in);
