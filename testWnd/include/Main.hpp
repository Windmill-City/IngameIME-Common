#pragma once

#include "Render.hpp"
#include "Resource.hpp"

#include <IngameIME.hpp>

struct MainContext
{
    static MainContext       Main;
    Renderer                 Render;
    GLFWwindow*              Window;
    IngameIME::InputContext* InputCtx;

    bool isFullscreen = false;

    void setup();
    void configFullscreen();
    void toggleFullscreen();
};