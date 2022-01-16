# IngameIME-Common

Common interface of IngameIME

## Class Diagram
``` mermaid
classDiagram
class PreEditContext{
    m_SelStart int32
    m_SelEnd int32
    m_Content wstring
}
class CompositionState{
    CompositionStart
    CompositionUpdate
    CompositionEnd
}
class PreEditCallback

class PreEditRect{
    m_Top int32
    m_Left int32
    m_Right int32
    m_Bottom int32
}
class PreEditRectCallback

class CandidateListContext{
    m_Selection int32
    m_Candidates list_wstring

    setSelection(int32)
    setFinalize()
}
class CandidateListState{
    CandidateListStart
    CandidateListUpdate
    CandidateListEnd
}
class CandidateListCallback

class CommitCallback

class ICallback{
    m_Callback functional_F
    setCallback(functional_F)
    runCallback(T...)
}
class Composition{
    terminate()
}
class InputContext{
    m_Composition Composition

    setActivated(bool)
    getActivated() bool
}
class InputProcessor{
    m_Type InputProcessorType
    m_Name: wstring
    m_Locale wstring
    m_LocaleName wstring

    setActivated()
}
class InputProcessorType{
    KeyboardLayout
    TextService
}
class InputProcessorContext{
    m_InputProcessor InputProcessor
    m_InputModes list_wstring
}
class IngameIME{
    getActiveInputProcessor() InputProcessorContext
    getInputProcessors() list<InputProcessor>
}

InputContext *-- Composition

PreEditCallback --> CompositionState
PreEditCallback --> PreEditContext
PreEditCallback ..|> ICallback

PreEditRectCallback --> PreEditRect
PreEditRectCallback ..|> ICallback

CandidateListCallback ..|> ICallback
CandidateListCallback --> CandidateListState
CandidateListCallback --> CandidateListContext

CommitCallback ..|> ICallback

Composition *-- PreEditCallback
Composition *-- PreEditRectCallback
Composition *-- CommitCallback
Composition *-- CandidateListCallback

InputProcessor *-- InputProcessorType

InputProcessorContext --> InputProcessor

IngameIME --> InputProcessorContext
IngameIME --> InputProcessor
```