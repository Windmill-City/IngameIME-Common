#pragma once

#include "ICallback.hpp"
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
        int32_t m_Left;
        int32_t m_Top;
        int32_t m_Right;
        int32_t m_Bottom;
    };

    /**
     * @brief Input Method position its Candidate Window near the PreEditRect
     *
     */
    typedef ICallback<PreEditRect&> PreEditRectCallback;

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
        int32_t m_SelStart;
        /**
         * @brief End index of the selection(exclusive)
         *
         */
        int32_t m_SetEnd;
        /**
         * @brief PreEdit text
         *
         */
        std::wstring m_Content;
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
    typedef ICallback<const CompositionState, const PreEditContext> PreEditCallback;
    /**
     * @brief Receive the convert result of the preEdit text
     *
     */
    typedef ICallback<const std::wstring> CommitCallback;

    struct CandidateListContext
    {
        /**
         * @brief Current selected Candidate
         *
         */
        int32_t m_Selection;
        /**
         * @brief Candidate strings
         *
         */
        std::list<std::wstring> m_Candidates;
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
    typedef ICallback<const CandidateListState, const CandidateListContext> CandidateListCallback;

    /**
     * @brief Composition
     *
     */
    class Composition : public PreEditCallback,
                        public PreEditRectCallback,
                        public CommitCallback,
                        public CandidateListCallback {
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
