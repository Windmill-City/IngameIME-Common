#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Renderer
{
    bool showDemo     = false;
    bool showDebugger = false;

    void setup();
    void newFrame();
    void destroy();
};