#pragma once

#include "ICallbackHolder.hpp"
#include <list>
#include <string>

namespace IngameIME {
    /**
     * @brief the boundary rectangle of the preedit, in window coordinate
     *
     * @note If the length of preedit is 0 (as it would be drawn by input method), the rectangle
     * coincides with the insertion point, and its width is 0.
     */
    struct PreEditRect
    {
        int32_t left;
        int32_t top;
        int32_t right;
        int32_t bottom;
    };

    /**
     * @brief Input Method position its Candidate Window near the PreEditRect
     *
     */
    using PreEditRectCallbackHolder = ICallbackHolder<PreEditRect&>;

    /**
     * @brief Context of the PreEdit text
     *
     */
    struct PreEditContext
    {
        /**
         * @brief Start index of the selection(inclusive)
         *
         */
        int32_t selStart;
        /**
         * @brief End index of the selection(exclusive)
         *
         */
        int32_t selEnd;
        /**
         * @brief PreEdit text
         *
         */
        std::wstring content;
    };

    /**
     * @brief Composition state
     *
     */
    enum class CompositionState { Begin, Update, End };

    /**
     * @brief PreEdit text callback, PreEditContext non-Null only when Composition::Update
     *
     */
    using PreEditCallbackHolder = ICallbackHolder<const CompositionState, const PreEditContext*>;
    /**
     * @brief Receive the convert result of the preEdit text
     *
     */
    using CommitCallbackHolder = ICallbackHolder<const std::wstring>;

    struct CandidateListContext
    {
        /**
         * @brief Current selected Candidate
         *
         */
        int32_t selection;
        /**
         * @brief Candidate strings
         *
         */
        std::list<std::wstring> candidates;
    };

    /**
     * @brief CandidateList state
     *
     */
    enum class CandidateListState { Begin, Update, End };

    /**
     * @brief Receive Candidate strings in fullscreen mode,
     * CandidateListContext non-Null only when CandidateList::Update
     *
     */
    using CandidateListCallbackHolder = ICallbackHolder<const CandidateListState, const CandidateListContext*>;

    /**
     * @brief Composition
     *
     */
    class Composition : public PreEditCallbackHolder,
                        public PreEditRectCallbackHolder,
                        public CommitCallbackHolder,
                        public CandidateListCallbackHolder {
      public:
        virtual ~Composition() = default;

      public:
        /**
         * @brief Terminate active composition
         *
         */
        virtual void terminate() = 0;
    };
}// namespace IngameIME
