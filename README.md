# IngameIME-Common

Common interface of IngameIME

## InputContext
To direct KeyEvent to the GameWindow
```c++
InputContext::setActivated(false)
```
To direct KeyEvent to the TextService

KeyboardLayout **always** handles KeyEvent whatever InputContext is activated or not
```c++
InputContext::setActivated(true)
```

## InputModes
For TextServices in Asia there are following InputModes available:

Mutually exclusive InputModes indicate how TextService should translate PreEdit text
- AlphaNumeric
- Native

Mutually exclusive InputModes indicate how TextService should translate punctuation marks
- HalfShape
- FullShape

Mutually exclusive InputModes indicate how Japanese TextService should translate PreEdit text
- Hiragana
- Katakana

## Bindings
Use [SWIG](https://github.com/swig/swig) to generate binding files

Currently supported bindings:
- Java
- CSharp
- Python
## Class Diagram
![Class Diagram](docs/ClassDiagram.svg)