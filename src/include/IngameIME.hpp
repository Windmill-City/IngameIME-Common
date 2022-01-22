#pragma once

#include "InputProcessor.hpp"
#include <list>

namespace IngameIME {
    class IngameIMECore {
      public:
        virtual ~IngameIMECore() = default;

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<InputProcessor> getActiveInputProcessor() = 0;
        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<InputProcessor>> getInputProcessors() = 0;
    };
}// namespace IngameIME
