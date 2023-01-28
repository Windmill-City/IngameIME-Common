# IngameIME-Common

Common interface of IngameIME

## Note

You must set active locale before calling any IngameIME methods, or you may get messy codes in CJK strings

```c++
// Pass empty string to set system default locale
setlocale(LC_ALL, "");
```

## InputContext

To pass KeyEvent to the GameWindow

```c++
InputContext::setActivated(false)
```

To pass KeyEvent to the TextService

```c++
InputContext::setActivated(true)
```

## InputModes

For TextServices in Asia there are following InputModes available:

Mutually exclusive InputModes indicate how TextService should convert PreEdit text

- AlphaNumeric
- Native

Mutually exclusive InputModes indicate how TextService should convert punctuation marks

- HalfShape
- FullShape

Mutually exclusive InputModes indicate how Japanese TextService should convert PreEdit text

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
