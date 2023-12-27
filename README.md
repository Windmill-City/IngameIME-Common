# IngameIME-Common

Common interface of IngameIME

## How to use

Example: <https://github.com/Windmill-City/IngameIME-Common/blob/main/testWnd/src/Render.cpp>

### Initialize

#### Win32

```c++
// IngameIME: Initialize
HWND hWnd;
// TextServiceFramework for Windows 8 or above
MainContext::Main.InputCtx = IngameIME::CreateInputContextWin32(hWnd, IngameIME::API::TextServiceFramework);
// Imm32 for Windows XP/Vista/7
MainContext::Main.InputCtx = IngameIME::CreateInputContextWin32(hWnd, IngameIME::API::Imm32);
```

### PreEdit & Commit & CandidateList

```c++
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
            //Render your preedit
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
            //render your candidate list
        });
    /**
     * @brief Receive the input mode change event, and show an indicator over the TextEdit
     *
     */
    inputCtx->IngameIME::InputModeCallbackHolder::setCallback(
        [](IngameIME::InputMode mode)
        {
            //render your input mode indicator
        });
}
```

### Update PreEdit Rect

```c++
MainContext::Main.InputCtx->setPreEditRect(Rect);
```

### Activate Input Method

```c++
MainContext::Main.InputCtx->setActivated(true/false);
```

### Fullscreen

```c++
MainContext::Main.InputCtx->setFullscreen(true/false);
```

## Bindings

Use [SWIG](https://github.com/swig/swig) to generate binding files

Currently supported bindings:

- Java
- CSharp
- Python
