#pragma once

#include <functional>

namespace IngameIME {
    template <typename... Args>
    class ICallbackHolder {
      private:
        std::function<void(Args...)> callback;

      public:
        /**
         * @brief Set callback
         *
         * @param callback callback to set
         * @return std::function<void(Args...)> previous callback
         */
        std::function<void(Args...)> setCallback(const std::function<void(Args...)> callback)
        {
            auto oldCallback = this->callback;
            this->callback       = callback;
            return oldCallback;
        }

        /**
         * @brief Run the callback
         *
         * @param args args pass to callback
         */
        void runCallback(const Args... args)
        {
            if (callback) callback(std::forward(args)...);
        }
    };
}// namespace IngameIME
