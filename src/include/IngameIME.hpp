#pragma once

#include "InputContext.hpp"
#include "InputProcessor.hpp"
#include <list>

namespace IngameIME {
    class Global : public InputProcessorCallbackHolder {
      public:
        static const std::string Version;

      public:
        static Global& getInstance(void* first = nullptr, ...);

      public:
        virtual ~Global() = default;

      public:
        /**
         * @brief Get Active InputProcessor
         *
         * @return std::shared_ptr<InputProcessor>
         */
        virtual std::shared_ptr<const InputProcessor> getActiveInputProcessor() const = 0;
        /**
         * @brief Get system availiable InputProcessor
         *
         * @return std::list<std::shared_ptr<InputProcessor>>
         */
        virtual std::list<std::shared_ptr<const InputProcessor>> getInputProcessors() const = 0;
        /**
         * @brief Get the InputContext object
         *
         * @return std::shared_ptr<InputContext>
         */
        virtual std::shared_ptr<InputContext> getInputContext(void* first, ...) = 0;
    };
}// namespace IngameIME
