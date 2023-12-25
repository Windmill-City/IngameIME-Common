cmake_minimum_required(VERSION 3.18)
project(nuklear)

add_library(nuklear INTERFACE)
target_include_directories(nuklear INTERFACE Nuklear Nuklear/demo/glfw_opengl4)
target_compile_definitions(nuklear INTERFACE
    NK_INCLUDE_FIXED_TYPES
    NK_INCLUDE_STANDARD_IO
    NK_INCLUDE_STANDARD_VARARGS
    NK_INCLUDE_DEFAULT_ALLOCATOR
    NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    NK_INCLUDE_FONT_BAKING
    NK_BUTTON_TRIGGER_ON_RELEASE
    NK_KEYSTATE_BASED_INPUT
)
