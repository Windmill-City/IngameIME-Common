#pragma once

#include <functional>

namespace IngameIME {
    template <typename... Args>
    class ICallback {
      private:
        std::function<void(Args...)> m_Callback;

      public:
        /**
         * @brief Set callback
         *
         * @param callback callback to set
         * @return std::function<void(Args...)> previous callback
         */
        std::function<void(Args...)> setCallback(const std::function<void(Args...)> callback)
        {
            auto oldCallback = m_Callback;
            m_Callback       = callback;
            return oldCallback;
        }

        /**
         * @brief Run the callback
         *
         * @param args args pass to callback
         */
        void runCallback(const Args... args)
        {
            if (m_Callback) m_Callback(std::forward(args)...);
        }
    };
}// namespace IngameIME
