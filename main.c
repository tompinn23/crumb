#include "display.h"

#include "glad/gl.h"
#include "log.h"

int main(int argc, char** argv) {
    display_ctx_t* ctx = display_open("Hello", 80, 50);

    unsigned int tile_shader = shader_load("tile.vert", "tile.frag");
    if(!tile_shader) {
        display_close(ctx);
        return -1;
    }

    while(!glfwWindowShouldClose(ctx->window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(ctx->window);
    }
    display_close(ctx);
    return 0;
}
