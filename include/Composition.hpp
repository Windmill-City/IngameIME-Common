#pragma once

#include "ICallbackHolder.hpp"
#include <list>
#include <string>

namespace IngameIME
{
/**
 * @brief Bounding box of the PreEdit text drawn by the Application, the position is in window coordinate
 *
 */
struct PreEditRect
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

/**
 * @brief Application receives the PreEdit text on CompositionState::Update, and draw it in its TextEdit contorl
 *
 */
struct PreEditContext
{
    /**
     * @brief Index of the selection start(inclusive)
     *
     */
    int32_t      selStart;
    /**
     * @brief Index of the selection end(exclusive)
     *
     */
    int32_t      selEnd;
    /**
     * @brief PreEdit text, Unicode encoded
     *
     */
    std::wstring content;
};

/**
 * @brief Composition state
 *
 */
enum class CompositionState
{
    Begin,
    Update,
    End
};

/**
 * @brief Application receives the PreEdit text on CompositionState::Update, and draw it in its TextEdit contorl
 *
 */
using PreEditCallbackHolder = ICallbackHolder<const CompositionState, const PreEditContext*>;
/**
 * @brief Application receives the convert result of the PreEdit text, and insert it into its TextEdit control
 *
 */
using CommitCallbackHolder  = ICallbackHolder<const std::wstring>;

/**
 * @brief Application receives Candidate strings in fullscreen mode, and draw the CandidateList itself
 *
 */
struct CandidateListContext
{
    /**
     * @brief Current selected Candidate
     *
     */
    int32_t                 selection;
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
enum class CandidateListState
{
    Begin,
    Update,
    End
};

/**
 * @brief Application receives Candidate strings in fullscreen mode, and draw the CandidateList itself
 *
 */
using CandidateListCallbackHolder = ICallbackHolder<const CandidateListState, const CandidateListContext*>;

/**
 * @brief Composition
 *
 */
class Composition
    : public PreEditCallbackHolder
    , public CommitCallbackHolder
    , public CandidateListCallbackHolder
{
  public:
    virtual ~Composition() = default;

  public:
    /**
     * @brief Bounding box of the PreEdit text drawn by the Application, the position is in window coordinate
     *
     */
    virtual void        setPreEditRect(const PreEditRect& rect) = 0;
    /**
     * @brief Bounding box of the PreEdit text drawn by the Application, the position is in window coordinate
     *
     */
    virtual PreEditRect getPreEditRect()                        = 0;
    /**
     * @brief Terminate active composition
     *
     */
    virtual void terminate() = 0;
};
} // namespace IngameIME
