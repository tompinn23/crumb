#include "display.h"

#include <string.h>

#include "glad/gl.h"
#include "log.h"
#include "z-mem.h"

terminal_t* terminal_open(display_ctx_t* ctx, int w, int h) {
    terminal_t* term = mem_zalloc(sizeof(*term));
    term->fg = 0xffffffff;
    term->bg = 0x00000000;
    term->ctx = ctx;
    term->glyphs = mem_zalloc(w * h * sizeof(glyph_t));
    term->vertices = mem_zalloc(w * h * 4 * sizeof(tile_vertex_t));
    term->indices = mem_alloc(w * h * 6 * sizeof(unsigned int));
    term->fg_buffer = mem_alloc(w * h * sizeof(uint32_t));
    memset(term->fg_buffer, 0xff, w * h * sizeof(uint32_t));
    term->bg_buffer = mem_zalloc(w * h * sizeof(uint32_t));
    term->dirty_cells = bitset_create();
    int j = 0;
    for(int i = 0; i < w * h * 6; i += 6) {
        term->indices[i] = j;
        term->indices[i + 1] = j + 1;
        term->indices[i + 2] = j + 2;
        term->indices[i + 3] = j + 1;
        term->indices[i + 4] = j + 2;
        term->indices[i + 5] = j + 3;
        j+= 4;
    }
    unsigned buf[3];
    glCreateBuffers(3, buf);
    term->vbo = buf[0];
    term->vao = buf[1];
    term->ibo = buf[2];
    glNamedBufferStorage(term->vbo, sizeof(tile_vertex_t) * w * h * 4, NULL,
            GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferStorage(term->ibo, sizeof(unsigned int) * w * h * 6,
            term->indices, GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayVertexBuffer(term->vao, 0, term->vbo, 0, sizeof(tile_vertex_t));

    glVertexArrayElementBuffer(term->vao, term->ibo);

    glEnableVertexArrayAttrib(term->vao, 0);
    glEnableVertexArrayAttrib(term->vao, 1);
    glEnableVertexArrayAttrib(term->vao, 2);

    glVertexArrayAttribFormat(term->vao, 0, 2, GL_FLOAT, GL_FALSE,
            offsetof(tile_vertex_t, pos));
    glVertexArrayAttribFormat(term->vao, 1, 2, GL_FLOAT, GL_FALSE,
            offsetof(tile_vertex_t, tex));
    glVertexArrayAttribFormat(term->vao, 2, 4, GL_FLOAT, GL_FALSE,
            offsetof(tile_vertex_t, colour));

    glVertexArrayAttribBinding(term->vao, 0, 0);
    glVertexArrayAttribBinding(term->vao, 1, 0);
    glVertexArrayAttribBinding(term->vao, 2, 0);


    glCreateTextures(GL_TEXTURE_2D, 1, &term->tex_fg);
    glCreateTextures(GL_TEXTURE_2D, 1, &term->tex_bg);
    glTextureStorage2D(term->tex_fg, 1, GL_RGBA8, w, h);
    glTextureStorage2D(term->tex_bg, 1, GL_RGBA8, w, h);

    glCreateFramebuffers(1, &term->fbo);
    glCreateTextures(GL_TEXTURE_2D, 1, &term->texture);
    glTextureParameteri(term->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(term->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(term->texture, 1, GL_RGBA8,
            w * DEF_TILESET_W, h * DEF_TILESET_H);
    glNamedFramebufferTexture(term->fbo, GL_COLOR_ATTACHMENT0,
            term->texture, 0);
    glCreateRenderbuffers(1, &term->rbo);
    glNamedRenderbufferStorage(term->rbo, GL_DEPTH24_STENCIL8,
            w * DEF_TILESET_W, h * DEF_TILESET_H);
    glNamedFramebufferRenderbuffer(term->rbo, GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER, term->rbo);
    if(glCheckNamedFramebufferStatus(term->fbo, GL_FRAMEBUFFER)
            != GL_FRAMEBUFFER_COMPLETE) {
        log_error("failed to create complete framebuffer");
        terminal_close(term);
        return NULL;
    }
    return term;
}

void terminal_close(terminal_t* terminal) {
    glDeleteTextures(1, &terminal->texture);
    glDeleteRenderbuffers(1, &terminal->rbo);
    glDeleteFramebuffers(1, &terminal->fbo);
    mem_free(terminal);
}


void terminal_blit(terminal_t* t) {
    if(bitset_count(t->dirty_cells) > 0) {
        size_t i = 0;
        for(i = 0; bitset_next(t->dirty_cells, &i); i++) {

        }
    }
}



