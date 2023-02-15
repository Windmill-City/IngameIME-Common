# IngameIME-Common

Common interface of IngameIME

## How to use

Example: <https://github.com/Windmill-City/IngameIME-Common/tree/main/testWnd>

### Initialize

#### Win32

You must set active locale before calling any IngameIME methods, or you may get messy codes in CJK strings

```c++
// Pass empty string to set system default locale
setlocale(LC_ALL, "");
```

```c++
// IngameIME: Initialize
HWND your_window_handle;
auto im = IngameIME::Global::getInstance().getInputContext(your_window_handle);
```

### PreEdit & Commit

```c++
// IngameIME: Receive commit text and insert it into your textedit
im->comp->IngameIME::CommitCallbackHolder::setCallback(
    [window](std::wstring commit)
    {
        // Insert commit into textedit
        your_textedit.insert(commit);
    });
// IngameIME: Receive PreEdit text and draw it in your textedit
im->comp->IngameIME::PreEditCallbackHolder::setCallback(
    [&preedit](IngameIME::CompositionState state, const IngameIME::PreEditContext* ctx)
    {
        switch (state)
        {
        case IngameIME::CompositionState::Begin:
            // Prepare for PreEdit text rendering
            your_preedit_renderer.reset_state();
        case IngameIME::CompositionState::End:
            // Cleanup PreEdit text rendering stuff
            your_preedit_renderer.cleanup();
            break;
        case IngameIME::CompositionState::Update:
            // Do PreEdit text rendering
            your_preedit_renderer.render(ctx->content, ctx->selStart, ctx->selEnd);
            break;
        }
    });
```

#### Update PreEdit Rect

```c++
Your_PreEdit_Renderer::layout()
{
    // IngameIME: Pass the bounder of the PreEdit text you are rendering
    // You should place the PreEdit text at your textedit's caret
    im->comp->setPreEditRect({Bounder.x, Bounder.y, Bounder.width, Bounder.height});
}
```

### Fullscreen Adaptation

If your game is in fullscreen mode, you need to do extra work:

#### State Indicator & Candidate List Drawing

```c++
//...
```

## Bindings

Use [SWIG](https://github.com/swig/swig) to generate binding files

Currently supported bindings:

- Java
- CSharp
- Python

## Class Diagram

![Class Diagram](docs/ClassDiagram.svg)
