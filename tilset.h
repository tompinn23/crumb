#pragma once

#include "cglm/struct.h"
#include "cglm/types-struct.h"
#include <stdint.h>
#include <stddef.h>

typedef struct tileset {
    ivec2s sprite_size;
    uint8_t* sprites;
    size_t len;
    size_t cap;

    uint32_t* codepoints;

    unsigned int gl_tex;
} tileset_t;

tileset_t* tileset_new(ivec2s sprite_size);
void tileset_delete(tileset_t* ts);

int tileset_reserve(tileset_t* ts, size_t new_size);

void tileset_upload_tile(tileset_t* ts, uint32_t cp, uint8_t* data);

