#pragma once

#include "Render.hpp"
#include "Resource.hpp"

struct MainContext
{
    static MainContext Main;
    ResourceManager    R;
    Renderer           Render;
    GLFWwindow*        Window;

    bool isFullscreen = false;

    void setup();
    void centerWindow();
    void toggleFullscreen();
};