#include "IngameIMEImpl.hpp"
#include "Main.hpp"

#ifdef _WINDOWS_
  #define GLFW_EXPOSE_NATIVE_WIN32
  #include <GLFW/glfw3native.h>
#endif

IngameIMEImpl IngameIMEImpl::IME = IngameIMEImpl();

void IngameIMEImpl::drawOverlay()
{
    // Place Top-left
    const float          PAD      = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2               work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
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
        ImVec2 PreEditPos = GImGui->PlatformImeData.InputPos;
        ImGui::Text("PreEdit Pos: %.f, %.f", PreEditPos.x, PreEditPos.y);
        ImGui::Text("InputMode: %s", InputMode == IngameIME::InputMode::Native ? "Native" : "AlphaNumeric");
        ImGui::Text("LastModeChange: %.f", LastInputModeChanged);
        if (MainContext::Main.InputCtx && MainContext::Main.InputCtx->getActivated())
            ImGui::Text("Activated: %s", "true");
        else
            ImGui::Text("Activated: %s", "false");
    }
    ImGui::End();
}
void IngameIMEImpl::installCallbacks()
{
    auto inputCtx = MainContext::Main.InputCtx;
    /**
     * @brief Receive PreEdit information and render it over the TextEdit
     *
     */
    inputCtx->IngameIME::PreEditCallbackHolder::setCallback(
        [this](const IngameIME::CompositionState state, const IngameIME::PreEditContext* ctx)
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
        [this](const IngameIME::CandidateListState state, const IngameIME::CandidateListContext* ctx)
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
     * @brief Receive the input mode change event, and show an indicator over the
     * TextEdit
     *
     */
    inputCtx->IngameIME::InputModeCallbackHolder::setCallback(
        [this](IngameIME::InputMode mode)
        {
            InputMode            = mode;
            LastInputModeChanged = glfwGetTime();
        });
}

void IngameIMEImpl::drawSelectorAPI()
{
    const char* items[] = {
        "GLFW-Default",
#ifdef _WINDOWS_
        "TextServiceFramework",
        "Imm32",
#endif
    };

    if (ImGui::BeginCombo("Choose API", items[SelectedAPI], 0))
    {
        for (int i = 0; i < IM_ARRAYSIZE(items); i++)
        {
            const bool is_selected = (SelectedAPI == i);
            if (ImGui::Selectable(items[i], is_selected)) SelectedAPI = i;

            // Set the initial focus when opening the combo (scrolling + keyboard
            // navigation focus)
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void IngameIMEImpl::updateAPI()
{
    if (ActiveAPI != SelectedAPI)
    {
        // Disable old api
        if (MainContext::Main.InputCtx)
        {
            delete MainContext::Main.InputCtx;
            MainContext::Main.InputCtx = nullptr;
        }

        // Destroy old window
        MainContext::Main.destroy();
        MainContext::Main.setup();

#ifdef _WINDOWS_
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
#endif

        // Register callbacks
        if (SelectedAPI > 0) installCallbacks();
        ActiveAPI = SelectedAPI;
    }
}

void IngameIMEImpl::drawTestWindow()
{
    ImGuiIO&         io           = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("IngameIME Test", NULL, window_flags))
    {
        drawSelectorAPI();

        static char text1[1024 * 8];
        ImGui::InputTextMultiline("##source1",
                                  text1,
                                  IM_ARRAYSIZE(text1),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                                  0);
        bool focused = ImGui::IsItemFocused();

        static char text2[1024 * 8];
        ImGui::InputTextMultiline("##source2",
                                  text2,
                                  IM_ARRAYSIZE(text2),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 8),
                                  0);
        focused |= ImGui::IsItemFocused();

        if (MainContext::Main.InputCtx && MainContext::Main.InputCtx->getActivated() != focused)
            MainContext::Main.InputCtx->setActivated(focused);

        // Center window
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        auto                 pos      = viewport->GetWorkCenter();
        pos.x -= ImGui::GetWindowWidth() / 2;
        pos.y -= ImGui::GetWindowHeight() / 2;
        ImGui::SetWindowPos(pos);
    }
    ImGui::End();

    if (PreEditCtx) MainContext::Main.Render.drawPreEdit(PreEditCtx);
    if (CandidateListCtx) MainContext::Main.Render.drawCandidateList(CandidateListCtx);
    if (PreEditCtx && glfwGetTime() - LastInputModeChanged < AutoHideDelay)
        MainContext::Main.Render.drawInputMode(InputMode);
}