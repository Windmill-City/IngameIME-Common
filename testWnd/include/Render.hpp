#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Renderer
{
    Renderer();
    ~Renderer();
    void setup();
    void newFrame();
};