#include "z-mem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
unsigned int mem_flags = 0;

#define SZ(uptr)	*((size_t *)((char *)(uptr) - sizeof(size_t)))

void die(const char* err, int code)
{
	fprintf(stderr, "Dieing because we are %s, with code %d\n", err, code);
	exit(code);
}

/*
* Allocate `len` bytes of memory.
*
* Returns:
*  - NULL if `len` == 0; or
*  - a pointer to a block of memory of at least `len` bytes
*
* Doesn't return on out of memory.
*/
void* mem_alloc(size_t len)
{
	char* mem;

	/* Allow allocation of "zero bytes" */
	if (len == 0) return (NULL);

	mem = malloc(len + sizeof(size_t));
	if (!mem)
		die("Out of Memory!", EXIT_FAILURE);
	mem += sizeof(size_t);
#if defined(POISON_ALLOC)
	memset(mem, 0xCC, len);
#endif
	SZ(mem) = len;

	return mem;
}

void* mem_zalloc(size_t len)
{
	void* mem = mem_alloc(len);
	memset(mem, 0, len);
	return mem;
}

void mem_free(void* p)
{
	if (!p) return;

#if defined(POISON_FREE)
	memset(p, 0xCD, SZ(p));
#endif
	free((char*)p - sizeof(size_t));
}

void* mem_realloc(void* p, size_t len)
{
	char* m = p;

	/* Fail gracefully */
	if (len == 0) return (NULL);

	m = realloc(m ? m - sizeof(size_t) : NULL, len + sizeof(size_t));
	m += sizeof(size_t);

	/* Handle OOM */
	if (!m) die("Out of Memory!", EXIT_FAILURE);
	SZ(m) = len;

	return m;
}

char* mem_strdup(const char* in) {
	char* copy;
	size_t sz = strlen(in);
	copy = mem_alloc(sz + 1);
	memcpy(copy, in, sz);
	*(copy + sz) = '\0';
	return copy;
}
