#pragma once

#include "ICallbackHolder.hpp"
#include <list>
#include <memory>
#include <string>

namespace IngameIME
{
enum class InputProcessorType
{
    KeyboardLayout,
    TextService
};

struct Locale
{
    /**
     * @brief Locale identifier
     *
     */
    std::wstring locale;
    /**
     * @brief Localized-Name of the Locale
     *
     */
    std::wstring name;
};

class InputProcessor
{
  public:
    InputProcessorType            type;
    /**
     * @brief Locale of the InputProcessor
     *
     */
    std::shared_ptr<const Locale> locale;
    /**
     * @brief Localized-Name of the InputProcessor
     *
     */
    std::wstring                  name;

  public:
    virtual ~InputProcessor() = default;

  public:
    /**
     * @brief Set active InputProcessor for current InputProcessor
     *
     */
    virtual void setActivated() const = 0;
};

enum class InputMode
{
    /**
     * @brief InputProcessor won't construct a composition when specific key being pressed,
     but converts them to specific char(similar to WM_CHAR) and pass them through CommitCallback
     */
    AlphaNumeric,
    /**
     * @brief InputProcessor construct a composition when specific key being pressed,
     or convert them to language specific Punctuation Mark
     */
    Native,
    /**
     * @brief Japanese InputProcessor specific PreEdit text convertion mode
     */
    Katakana,
    /**
     * @brief Japanese InputProcessor specific PreEdit text convertion mode
     */
    Hiragana,
    /**
     * @brief Punctuation Mark conversion mode
     */
    FullShape,
    /**
     * @brief Punctuation Mark conversion mode
     */
    HalfShape
};

struct InputProcessorContext
{
    /**
     * @brief Current active InputProcessor
     *
     */
    std::shared_ptr<const InputProcessor> proc;
    /**
     * @brief InputModes of the InputProcessor
     *
     */
    std::list<InputMode>                  modes;
};

enum class InputProcessorState
{
    /**
     * @brief Only InputMode has changed
     *
     */
    InputModeUpdate,
    /**
     * @brief Both InputProcessor and InputMode has changed
     *
     */
    FullUpdate
};

using InputProcessorCallbackHolder = ICallbackHolder<const InputProcessorState, const InputProcessorContext&>;
} // namespace IngameIME