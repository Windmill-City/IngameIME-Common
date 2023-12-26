#include "Render.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Main.hpp"

static ImGuiContext*                    g;
static IngameIME::PreEditContext*       PreEditCtx;
static IngameIME::CandidateListContext* CandidateListCtx;
static IngameIME::InputMode             Mode           = IngameIME::InputMode::AlphaNumeric;
static double                           LastModeChange = 0;
// Config how long the InputMode displays
static double                           AutoHideDelay  = 3;

void OverlayTextV(const char* fmt, va_list args)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    ImGuiContext&     g     = *GImGui;
    const ImGuiStyle& style = g.Style;

    const char *text_beg, *value_text_end;
    ImFormatStringToTempBufferV(&text_beg, &value_text_end, fmt, args);
    const ImVec2 text_size = ImGui::CalcTextSize(text_beg, value_text_end, false);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect bb(pos, pos + ImVec2(text_size.x + style.FramePadding.x * 2, text_size.y + style.FramePadding.y * 2));
    ImGui::ItemSize(bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, 0)) return;

    // Render
    const ImU32 bg_col = ImGui::GetColorU32(ImGuiCol_WindowBg);
    window->DrawList->AddRectFilled(bb.GetTL(), bb.GetBR(), bg_col);
    ImGui::RenderTextClipped(bb.Min + style.FramePadding,
                             bb.Max,
                             text_beg,
                             value_text_end,
                             &text_size,
                             ImVec2(0.0f, 0.0f));
}

void OverlayText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    OverlayTextV(fmt, args);
    va_end(args);
}

static void InfoOverlay()
{
    // Place Top-left
    const float          PAD       = 10.0f;
    const ImGuiViewport* viewport  = ImGui::GetMainViewport();
    ImVec2               work_pos  = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2               work_size = viewport->WorkSize;
    ImVec2               window_pos;
    window_pos.x = work_pos.x + PAD;
    window_pos.y = work_pos.y + PAD;
    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

    if (ImGui::Begin("Overlay",
                     NULL,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration))
    {
        ImGui::Text("KeyBinding");
        ImGui::Separator();
        ImGui::Text("F7  - Toggle Demo");
        ImGui::Text("F11 - Toggle Fullscreen");
        ImGui::Text("F12 - Toggle Debugger");

        ImGui::Separator();
        ImVec2 PreEditPos = g->PlatformImeData.InputPos;
        ImGui::Text("PreEdit Pos: %.f, %.f", PreEditPos.x, PreEditPos.y);
        ImGui::Text("InputMode: %s", Mode == IngameIME::InputMode::Native ? "Native" : "AlphaNumeric");
        ImGui::Text("LastModeChange: %.f", LastModeChange);
        if (MainContext::Main.InputCtx && MainContext::Main.InputCtx->getActivated())
            ImGui::Text("Activated: %s", "true");
        else
            ImGui::Text("Activated: %s", "false");
    }
    ImGui::End();
}

void RenderPreEdit()
{
    if (PreEditCtx)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {0, 0});
        ImVec2 PreEditPos = g->PlatformImeData.InputPos;
        ImGui::SetNextWindowPos(PreEditPos);
        if (ImGui::Begin("PreEdit",
                         NULL,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
                             | ImGuiWindowFlags_NoFocusOnAppearing))
        {
            // Before selected
            std::string bef_selected = PreEditCtx->content.substr(0, PreEditCtx->selStart);
            OverlayText(bef_selected.c_str());

            //  Selected Part
            std::string selected =
                PreEditCtx->content.substr(PreEditCtx->selStart, PreEditCtx->selEnd - PreEditCtx->selStart);
            if (selected.empty()) selected = " "; // Cursor

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.600f, 0.600f, 0.600f, 1.000f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));

            ImGui::SameLine();
            OverlayText(selected.c_str());

            ImGui::PopStyleColor(2);

            // After selected
            std::string aft_selected = PreEditCtx->content.substr(PreEditCtx->selEnd);
            ImGui::SameLine();
            OverlayText(aft_selected.c_str());

            ImGui::BringWindowToDisplayFront(g->CurrentWindow);
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}

void RenderCandidateList()
{
    if (CandidateListCtx)
    {
        ImVec2 CandidateListPos = g->PlatformImeData.InputPos;
        CandidateListPos.y += g->PlatformImeData.InputLineHeight + 5;
        ImGui::SetNextWindowPos(CandidateListPos);
        if (ImGui::Begin("CandidateList",
                         NULL,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
                             | ImGuiWindowFlags_NoFocusOnAppearing))
        {
            int i = 0;
            for (std::string& s : CandidateListCtx->candidates)
            {
                bool selected = i++ == CandidateListCtx->selection;
                if (selected)
                {
                    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.600f, 0.600f, 0.600f, 1.000f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));
                    OverlayText("%d. %s", i, s.c_str());
                    ImGui::PopStyleColor(2);
                }
                else
                    ImGui::Text("%d. %s", i, s.c_str());
                ImGui::SameLine();
            }
            ImGui::BringWindowToDisplayFront(g->CurrentWindow);
        }
        ImGui::End();
    }
}

void RenderInputMode()
{
    // Incase overlap
    if (!PreEditCtx && glfwGetTime() - LastModeChange < AutoHideDelay)
    {
        ImVec2 PreEditPos = g->PlatformImeData.InputPos;
        ImGui::SetNextWindowPos(PreEditPos);

        if (ImGui::Begin("InputMode",
                         NULL,
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
                             | ImGuiWindowFlags_NoFocusOnAppearing))
        {
            if (Mode == IngameIME::InputMode::Native)
                // Native Mode
                OverlayText("N");
            else
                // AlphaNumeric Mode
                OverlayText("A");

            ImGui::BringWindowToDisplayFront(g->CurrentWindow);
        }
        ImGui::End();
    }
}

void UpdatePreEditRect()
{
    if (MainContext::Main.InputCtx)
    {
        ImVec2 PreEditPos = g->PlatformImeData.InputPos;
        float  LineHeight = g->PlatformImeData.InputLineHeight + 5;

        IngameIME::PreEditRect Rect;
        Rect.x      = (int)PreEditPos.x;
        Rect.y      = (int)PreEditPos.y;
        Rect.height = (int)LineHeight;
        Rect.width  = 1; // at least one pixel
        MainContext::Main.InputCtx->setPreEditRect(Rect);
    }
}

void IngameIME_Install_Callbacks()
{
    auto inputCtx = MainContext::Main.InputCtx;
    /**
     * @brief Receive PreEdit information and render it over the TextEdit
     *
     */
    inputCtx->IngameIME::PreEditCallbackHolder::setCallback(
        [](const IngameIME::CompositionState state, const IngameIME::PreEditContext* ctx)
        {
            static IngameIME::PreEditContext Internal;
            if (ctx)
            {
                Internal   = *ctx;
                PreEditCtx = &Internal;
            }
            else
                PreEditCtx = nullptr;
        });

    /**
     * @brief Receive the converted text and insert it into the TextEdit
     *
     */
    inputCtx->IngameIME::CommitCallbackHolder::setCallback([](std::string commit)
                                                           { ImGui::GetIO().AddInputCharactersUTF8(commit.c_str()); });
    /**
     * @brief Receive the CandidateList and draw it over the TextEdit
     *
     */
    inputCtx->IngameIME::CandidateListCallbackHolder::setCallback(
        [](const IngameIME::CandidateListState state, const IngameIME::CandidateListContext* ctx)
        {
            static IngameIME::CandidateListContext Internal;
            if (ctx)
            {
                Internal         = *ctx;
                CandidateListCtx = &Internal;
            }
            else
                CandidateListCtx = nullptr;
        });
    /**
     * @brief Receive the input mode change event, and show an indicator over the TextEdit
     *
     */
    inputCtx->IngameIME::InputModeCallbackHolder::setCallback(
        [](IngameIME::InputMode mode)
        {
            Mode           = mode;
            LastModeChange = glfwGetTime();
        });
}

void IngameIME_API_Selector()
{
    const char* items[] = {
        "GLFW-Default",
        "TextServiceFramework",
        "Imm32",
    };

    static int ActiveAPI   = 0;
    static int SelectedAPI = 0;
    if (ImGui::BeginCombo("Choose API", items[SelectedAPI], NULL))
    {
        for (int i = 0; i < IM_ARRAYSIZE(items); i++)
        {
            const bool is_selected = (SelectedAPI == i);
            if (ImGui::Selectable(items[i], is_selected)) SelectedAPI = i;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ActiveAPI != SelectedAPI)
    {
        // Disable old api
        if (MainContext::Main.InputCtx)
        {
            delete MainContext::Main.InputCtx;
            MainContext::Main.InputCtx = nullptr;
        }

        HWND hWnd = glfwGetWin32Window(MainContext::Main.Window);

        // Enable new API
        switch (SelectedAPI)
        {
        case 0: break;
        case 1:
            MainContext::Main.InputCtx = IngameIME::CreateInputContextWin32(hWnd, IngameIME::API::TextServiceFramework);
            break;
        case 2: MainContext::Main.InputCtx = IngameIME::CreateInputContextWin32(hWnd, IngameIME::API::Imm32); break;
        }

        // Register callbacks
        if (SelectedAPI > 0) IngameIME_Install_Callbacks();
        ActiveAPI = SelectedAPI;
    }
}

void IngameIME_Test()
{
    ImGuiIO&         io           = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("IngameIME Test", NULL, window_flags))
    {
        IngameIME_API_Selector();

        static char text1[1024 * 8];
        ImGui::InputTextMultiline("##source1",
                                  text1,
                                  IM_ARRAYSIZE(text1),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                                  NULL);
        bool focused = ImGui::IsItemFocused();

        static char text2[1024 * 8];
        ImGui::InputTextMultiline("##source2",
                                  text2,
                                  IM_ARRAYSIZE(text2),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                                  NULL);
        focused |= ImGui::IsItemFocused();

        if (MainContext::Main.InputCtx && MainContext::Main.InputCtx->getActivated() != focused)
            MainContext::Main.InputCtx->setActivated(focused);

        UpdatePreEditRect();
        RenderPreEdit();
        RenderCandidateList();
        RenderInputMode();
    }
    ImGui::End();
}

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
    g           = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui_ImplGlfw_InitForOpenGL(MainContext::Main.Window, true);
    ImGui_ImplOpenGL3_Init();

    ImFontConfig config;
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf",
                                 16,
                                 &config,
                                 io.Fonts->GetGlyphRangesChineseFull());
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf", 16, &config, io.Fonts->GetGlyphRangesJapanese());
    io.Fonts->AddFontFromFileTTF("assets/fonts/unifont-15.1.04.otf", 16, &config, io.Fonts->GetGlyphRangesKorean());
}

void Renderer::newFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDemo) ImGui::ShowDemoWindow(&showDemo);
    if (showDebugger) ImGui::ShowMetricsWindow(&showDebugger);

    InfoOverlay();
    IngameIME_Test();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
