#include "Render.hpp"
#include "IngameIMEImpl.hpp"

#include "Main.hpp"

void drawTextV(const char* fmt, va_list args)
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

void Renderer::drawText(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    drawTextV(fmt, args);
    va_end(args);
}

void Renderer::drawPreEdit(IngameIME::PreEditContext* ctx)
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
        std::string bef_selected = ctx->content.substr(0, ctx->selStart);
        drawText(bef_selected.c_str());

        //  Selected Part
        std::string selected = ctx->content.substr(ctx->selStart, ctx->selEnd - ctx->selStart);
        if (selected.empty()) selected = " "; // Cursor

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.600f, 0.600f, 0.600f, 1.000f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));

        ImGui::SameLine();
        drawText(selected.c_str());

        ImGui::PopStyleColor(2);

        // After selected
        std::string aft_selected = ctx->content.substr(ctx->selEnd);
        ImGui::SameLine();
        drawText(aft_selected.c_str());

        ImGui::BringWindowToDisplayFront(g->CurrentWindow);
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void Renderer::drawCandidateList(IngameIME::CandidateListContext* ctx)
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
        for (std::string& s : ctx->candidates)
        {
            bool selected = i++ == ctx->selection;
            if (selected)
            {
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.600f, 0.600f, 0.600f, 1.000f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.000f, 0.000f, 0.000f, 1.000f));
                drawText("%d. %s", i, s.c_str());
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

void Renderer::drawInputMode(IngameIME::InputMode mode)
{
    ImVec2 PreEditPos = g->PlatformImeData.InputPos;
    ImGui::SetNextWindowPos(PreEditPos);
    ImGui::SetNextWindowBgAlpha(0.80f); // Transparent background

    if (ImGui::Begin("InputMode",
                     NULL,
                     ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
                         | ImGuiWindowFlags_NoFocusOnAppearing))
    {
        if (mode == IngameIME::InputMode::Native)
            // Native Mode
            ImGui::Text("N");
        else
            // AlphaNumeric Mode
            ImGui::Text("A");

        ImGui::BringWindowToDisplayFront(g->CurrentWindow);
    }
    ImGui::End();
}

void Renderer::setup()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    g           = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
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

void Renderer::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Renderer::newFrame()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDemo) ImGui::ShowDemoWindow(&showDemo);
    if (showDebugger) ImGui::ShowMetricsWindow(&showDebugger);

    IngameIMEImpl::IME.drawOverlay();
    IngameIMEImpl::IME.drawTestWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    IngameIMEImpl::IME.updateAPI();
}
