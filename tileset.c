#include "tilset.h"

#include "glad/gl.h"

#include "z-mem.h"

tileset_t* tileset_new(ivec2s sprite_size) {
    tileset_t* ret = mem_zalloc(sizeof(*ret));
    ret->sprite_size = sprite_size;
    return ret;
}

void tileset_delete(tileset_t* ts) {
    if(ts->sprites != NULL) {
        mem_free(ts->sprites);
    }
    if(ts->codepoints != NULL) {
        mem_free(ts->codepoints);
    }
    if(ts->gl_tex != 0) {
        glDeleteTextures(1, &ts->gl_tex);
    }
    mem_free(ts);
}

int tileset_reserve(tileset_t* ts, size_t new_size) {
    if(new_size < ts->cap) {
        return 1;
    }
    mem_realloc(ts->sprites, new_size * sizeof(uint8_t) * ts->sprite_size.x * ts->sprite_size.y);



    return 1;
}
