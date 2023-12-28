#pragma once
#include <IngameIME.hpp>

struct IngameIMEImpl
{
    static IngameIMEImpl IME;

    IngameIME::PreEditContext*       PreEditCtx;
    IngameIME::CandidateListContext* CandidateListCtx;
    IngameIME::InputMode             InputMode            = IngameIME::InputMode::AlphaNumeric;
    double                           LastInputModeChanged = 0;

    // Config how long the InputMode displays
    double AutoHideDelay = 3;
    int    ActiveAPI     = 0;
    int    SelectedAPI   = 0;

    void drawOverlay();
    void drawTestWindow();
    void drawSelectorAPI();
    void updateAPI();
    void installCallbacks();

    void updatePreEditRect();
};