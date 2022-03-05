#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "cglm/struct.h"

#include "bitset.h"

#define DEF_TILESET_W 10
#define DEF_TILESET_H 10

typedef struct render_queue {
    void* terminal;
    struct render_queue* next;
} render_queue_t;

typedef struct {
    GLFWwindow* window;
} display_ctx_t;

typedef struct {
    uint32_t ch;
    uint32_t fg;
    uint32_t bg;
} glyph_t;

typedef struct {
    vec2s pos, tex;
    vec4s colour;
} tile_vertex_t;

typedef struct {
    display_ctx_t* ctx;
    glyph_t* glyphs;
    bitset_t* dirty_cells;

    uint32_t fg;
    uint32_t bg;
    unsigned int fbo, texture, rbo;
    unsigned int vao, vbo, ibo;
    unsigned int tex_fg, tex_bg;

    tile_vertex_t* vertices;
    unsigned int* indices;
    uint32_t* fg_buffer;
    uint32_t* bg_buffer;
} terminal_t;

display_ctx_t* display_open(const char* title, int w, int h);
void display_close(display_ctx_t* ctx);

terminal_t* terminal_open(display_ctx_t* ctx, int w, int h);
void terminal_close(terminal_t* terminal);

unsigned int shader_load(const char* vpath, const char* fpath);
unsigned int shader_load_mem(char* vbuf, char* fbuf, size_t vlen, size_t flen);
