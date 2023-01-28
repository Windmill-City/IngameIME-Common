# IngameIME-Common

Common interface of IngameIME

## Note

You must set active locale before calling any IngameIME methods, or you may get messy codes in CJK strings

```c++
// Pass empty string to set system default locale
setlocale(LC_ALL, "");
```

## InputContext

To direct KeyEvent to the GameWindow

```c++
InputContext::setActivated(false)
```

To direct KeyEvent to the TextService

```c++
InputContext::setActivated(true)
```

KeyboardLayout **always** handles KeyEvent whatever InputContext is activated or not

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
