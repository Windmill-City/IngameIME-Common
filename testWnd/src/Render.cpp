#include "Render.hpp"

#include "Main.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <vector>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::setup()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplGlfw_InitForOpenGL(MainContext::Main.Window, true);
    ImGui_ImplOpenGL3_Init();

    ImFontConfig config;
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf", 16, &config, io.Fonts->GetGlyphRangesDefault());
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf",
                                 16,
                                 &config,
                                 io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf", 16, &config, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf", 16, &config, io.Fonts->GetGlyphRangesKorean());
}

void Renderer::newFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    bool show = true;
    ImGui::ShowMetricsWindow(&show);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
