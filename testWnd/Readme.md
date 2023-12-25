# IngameIME Test

IngameIME Test

## Encoding

### Glossary

| Word      | Meaning                                      | Example               |
| --------- | -------------------------------------------- | --------------------- |
| Codepoint | Numerical value maps to a specific character | \u0000                |
| Codepage  | Collection of codepoints                     | GBK; GB2312; Unicode; |
| Encoding  | Codepoint format in bytes                    | UTF-8; UTF-16; ANSI;  |
---

### Common Regulations

Following table shows the encoding of the strings depends on their physical location:

| Physical Location | Encoding |
| ----------------- | -------- |
| Network           | UTF-8    |
| Disk              | UTF-8    |
| Memory            | UTF-8    |

Note: Encoding here is **NOT** regulated by any standard! This regulation only suits in **MY** program!

Following table shows the encoding of the strings depends on their storage class:

| Class                 | Encoding |
| --------------------- | -------- |
| std::string           | UTF-8    |
| std::wstring(Windows) | UTF-16   |
| std::wstring(Linux)   | UTF-32   |
| std::u8string         | UTF-8    |
| std::u16string        | UTF-16   |
| std::u32string        | UTF-32   |

Note: Encoding here is **NOT** regulated by any standard! This regulation only suits in **MY** program!

Note: These classes knows nothing about encodings, so they **CAN'T** deal with **Variable Length** encodings.

### Locale

Before call into any locale dependent C APIs, you need to set locale first, for ensuring correct codepage and encoding are used.

```c++
setlocale(LC_ALL, ".UTF-8");
```

Then the locale-dependent functions, like `mbstowcs` or `wcstombs`, can properly translate strings.

Note: Do not forget to set the compiler encoding, for MSVC it is `/utf8`.

### std::locale::global

The following code is **Needless** to call, as the default locale is `C`, which means the program will use the same locale as the C environment, which we have set above.

```c++
std::locale::global(std::locale(".UTF-8"));
```

### Common Character Sets

#### ANSI

ANSI is an encoding method that regulates the conversion of codepoints to bytes and vice versa. To identify a specific character using its codepoints, it is necessary to know the codepage being used.

Language specific codepages:
| Codepage | Language            |
| -------- | ------------------- |
| GB2312   | Simplified Chinese  |
| GBK      | Simplified Chinese  |
| GB18030  | Simplified Chinese  |
| Big5     | Traditional Chinese |
| S-JIS    | Japanese            |

ANSI codepages can be different on different computers, or can be changed for a single computer, leading to data corruption. For the most consistent results, applications should use Unicode.

#### Unicode

While ANSI uses same encoding method across different codepages, Unicode has unique codepage but has different encodings.

Encodings:
| Encoding      | Length   | Range                 | Codepoint Size(Byte) |
| ------------- | -------- | --------------------- | -------------------- |
| UTF-8         | Variable | Full Unicode Codepage | 1 ~ 6                |
| UCS-2         | Fixed    | \u0000 ~ \uFFFF       | 2                    |
| UTF-16        | Variable | Full Unicode Codepage | 2 / 4                |
| UTF-32(UCS-4) | Fixed    | Full Unicode Codepage | 4                    |

## Reference

<https://learnopengl-cn.github.io/intro/>
