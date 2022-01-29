#pragma once

#include "InputContext.hpp"
#include "InputProcessor.hpp"
#include <list>

namespace IngameIME {
    class Global : public InputProcessorCallbackHolder {
      public:
        virtual ~Global() = default;

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<InputProcessor> getActiveInputProcessor() const = 0;
        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<InputProcessor>> getInputProcessors() const = 0;
        /**
         * @brief Get the InputContext object
         *
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<InputContext> getInputContext(...);
    };
    const Global& global;
}// namespace IngameIME
