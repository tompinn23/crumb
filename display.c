#include "display.h"
#include "glad/gl.h"

#include "z-mem.h"
#include "log.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define DEF_TILESET_W 10
#define DEF_TILESET_H 10


display_ctx_t* display_open(const char* title, int w, int h) {
    if(!glfwInit()) {
        log_error("failed to load glfw");
        return NULL;
    }
    GLFWwindow* window = glfwCreateWindow(DEF_TILESET_W * w, 
            DEF_TILESET_H * h, title, NULL, NULL);
    if(!window) {
        glfwTerminate();
        log_error("failed to create window");
        return NULL;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    display_ctx_t* ctx = mem_zalloc(sizeof(*ctx));
    ctx->window = window;
    return ctx;
}

void display_close(display_ctx_t* ctx) {
    glfwDestroyWindow(ctx->window);
    mem_free(ctx);
    glfwTerminate();
}

unsigned int shader_load(const char* vpath, const char* fpath) {
    errno = 0;
    struct stat st;
    if(lstat(vpath, &st) < 0) {
        log_error("failed to determine file size: %s %s", vpath, strerror(errno));
        return 0;
    }
    size_t vlen = st.st_size;
    if(lstat(fpath, &st) < 0) {
        log_error("failed to determine file size: %s %s", fpath, strerror(errno));
        return 0;
    }
    size_t flen = st.st_size;
    FILE *v, *f;
    v = fopen(vpath, "rb");
    if(!v) {
        log_error("failed to open vertex shader: %s, %s", vpath, strerror(errno));
        return 0;
    }
    f = fopen(fpath, "rb");
    if(!f) {
        log_error("failed to open fragment shader: %s %s", fpath, strerror(errno));
        fclose(v);
        return 0;
    }
    char *vbuf, *fbuf;
    vbuf = mem_alloc(vlen);
    fbuf = mem_alloc(flen);
    size_t ret;

    if((ret =fread(vbuf, 1, vlen, v)) != vlen) {
        log_error("failed to read file: %s", vpath);
        fclose(v);
        fclose(f);
        return 0;
    }
    if((ret =fread(fbuf, 1, flen, f)) != flen) {
        log_error("failed to read file: %s", fpath);
        fclose(v);
        fclose(f);
        return 0;
    }
    unsigned int shader = shader_load_mem(vbuf, fbuf, vlen, flen);
    fclose(v);
    fclose(f);
    mem_free(vbuf);
    mem_free(fbuf);
    return shader;
}

unsigned int shader_load_mem(char* vbuf, char* fbuf, size_t vlen, size_t flen) {
    unsigned int vertex, fragment;
    int ret;
    char log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vbuf, (int*)&vlen);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &ret);
    if(!ret) {
        glGetShaderInfoLog(vertex, 512, NULL, log);
        log_error("failed to compile vertex shader: %s", log);
        return 0;
    }
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fbuf, (int*)&flen);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &ret);
    if(!ret) {
        glGetShaderInfoLog(fragment, 512, NULL, log);
        log_error("failed to compile fragment shader: %s", log);
        return 0;
    }
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &ret);
    if(!ret) {
        glGetProgramInfoLog(program, 512, NULL, log);
        log_error("failed to link shader program: %s", log);
        return 0;
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}
