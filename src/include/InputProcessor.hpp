#pragma once

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
        std::wstring m_Locale;
        /**
         * @brief Localized-Name of the Locale
         *
         */
        std::wstring m_Name;
    };

    class InputProcessor {
      public:
        InputProcessorType m_Type;
        /**
         * @brief Locale of the InputProcessor
         *
         */
        std::shared_ptr<Locale> m_Locale;
        /**
         * @brief Localized-Name of the InputProcessor
         *
         */
        std::wstring m_Name;

      public:
        virtual ~InputProcessor() = default;

      public:
        /**
         * @brief Set active InputProcessor for current InputProcessor
         *
         */
        virtual void setActivated() = 0;
    };

    struct InputProcessorContext
    {
        /**
         * @brief Current active InputProcessor
         *
         */
        std::shared_ptr<InputProcessor> m_InputProcessor;
        /**
         * @brief InputModes of the InputProcessor
         *
         */
        std::list<std::wstring> m_InputModes;
    };
}// namespace IngameIME