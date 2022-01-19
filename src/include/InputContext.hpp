#pragma once

#include "Composotion.hpp"
#include <memory>

namespace IngameIME {
    class InputContext {
      public:
        std::shared_ptr<Composition> m_Composition;

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
        virtual bool getActivated() = 0;
    };
}// namespace IngameIME
