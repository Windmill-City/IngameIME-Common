#pragma once

#include "ICallbackHolder.hpp"
#include <list>
#include <memory>
#include <string>

namespace IngameIME {
    enum class InputProcessorType { KeyboardLayout, TextService };

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

    class InputProcessor {
      public:
        InputProcessorType type;
        /**
         * @brief Locale of the InputProcessor
         *
         */
        std::shared_ptr<const Locale> locale;
        /**
         * @brief Localized-Name of the InputProcessor
         *
         */
        std::wstring name;

      public:
        virtual ~InputProcessor() = default;

      public:
        /**
         * @brief Set active InputProcessor for current InputProcessor
         *
         */
        virtual void setActivated() const = 0;
    };

    struct InputProcessorContext
    {
        /**
         * @brief Current active InputProcessor
         *
         */
        std::shared_ptr<const InputProcessor> inputProcessor;
        /**
         * @brief InputModes of the InputProcessor
         *
         */
        std::list<std::wstring> inputModes;
    };

    enum class InputProcessorState {
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
}// namespace IngameIME