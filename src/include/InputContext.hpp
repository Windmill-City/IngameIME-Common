#pragma once

#include "Composition.hpp"
#include <memory>

namespace IngameIME {
    class InputContext {
      public:
        const std::shared_ptr<Composition> m_Composition;

      public:
        virtual ~InputContext() = default;

      public:
        /**
         * @brief Set InputContext activate state
         *
         * @param activated if InputContext activated
         */
        virtual void setActivated(bool activated) = 0;
        /**
         * @brief Get if InputContext activated
         *
         * @return true activated
         * @return false not activated
         */
        virtual bool getActivated() const = 0;
        /**
         * @brief Set InputContext full screen state
         *
         * @param fullscreen if InputContext full screen
         */
        virtual void setFullScreen(bool fullscreen) = 0;
        /**
         * @brief Get if InputContext in full screen state
         *
         * @return true full screen mode
         * @return false window mode
         */
        virtual bool getFullScreen() const = 0;
    };
}// namespace IngameIME
