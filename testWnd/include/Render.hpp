#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "IngameIME.hpp"

struct Renderer
{
    ImGuiContext* g;
    bool          showDemo     = false;
    bool          showDebugger = false;

    void setup();
    void destroy();
    void newFrame();

    void drawText(const char* fmt, ...);
    void drawPreEdit(IngameIME::PreEditContext* ctx);
    void drawCandidateList(IngameIME::CandidateListContext* ctx);
    void drawInputMode(IngameIME::InputMode mode);
};