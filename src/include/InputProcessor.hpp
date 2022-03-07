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
        std::string locale;
        /**
         * @brief Localized-Name of the Locale
         *
         */
        std::string name;
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
        std::string name;

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
        std::shared_ptr<const InputProcessor> proc;
        /**
         * @brief InputModes of the InputProcessor
         *
         */
        std::list<std::string> modes;
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