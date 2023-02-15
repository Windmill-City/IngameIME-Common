#pragma once

#include "Composition.hpp"
#include <memory>

namespace IngameIME
{
class InputContext
{
  public:
    std::shared_ptr<Composition> comp;

  public:
    virtual ~InputContext() = default;

  public:
    /**
     * @brief Set InputMethod active state
     *
     * @param activated if InputMethod activated
     */
    virtual void setActivated(const bool activated)   = 0;
    /**
     * @brief Get if InputMethod activated
     *
     * @return true activated
     * @return false not activated
     */
    virtual bool getActivated() const                 = 0;
    /**
     * @brief Set InputMethod full screen state
     *
     * @param fullscreen if InputMethod full screen
     */
    virtual void setFullScreen(const bool fullscreen) = 0;
    /**
     * @brief Get if InputMethod in full screen state
     *
     * @return true full screen mode
     * @return false window mode
     */
    virtual bool getFullScreen() const                = 0;
};
} // namespace IngameIME
