#pragma once

#include <functional>

namespace IngameIME
{
template <typename... Args>
class ICallbackHolder
{
  private:
    std::function<void(Args...)> callback;

  public:
    /**
     * @brief set callback
     *
     * @param callback callback to set
     * @return std::function<void(args...)> previous callback
     */
    std::function<void(Args...)> setCallback(const std::function<void(Args...)> callback)
    {
        auto oldCallback = this->callback;
        this->callback   = callback;
        return oldCallback;
    }

    /**
     * @brief get callback
     *
     * @return std::function<void(args...)> current callback
     */
    std::function<void(Args...)> getCallback()
    {
        return callback;
    }

    /**
     * @brief Run the callback
     *
     * @param args args pass to callback
     */
    void runCallback(Args... args)
    {
        if (callback) callback(args...);
    }
};
} // namespace IngameIME
