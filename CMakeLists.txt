cmake_minimum_required(VERSION 3.20)

project(crumb)

set(CMAKE_C_STANDARD 11)

add_subdirectory(libs/glfw-3.3.6)
add_subdirectory(libs/cglm-0.8.4)

add_library(glad OBJECT libs/glad/gl.c)
target_include_directories(glad PUBLIC libs/glad)

add_executable(crumb main.c display.c terminal.c bitset.c z-mem.c log.c)
target_link_libraries(crumb glfw glad cglm)

