#pragma once

#include <list>
#include <memory>
#include <string>

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
     * @return previous callback
     */
    std::function<void(Args...)> setCallback(const std::function<void(Args...)> callback)
    {
        auto oldCallback = this->callback;
        this->callback   = callback;
        return oldCallback;
    }

    /**
     * @return active callback
     */
    std::function<void(Args...)> getCallback()
    {
        return callback;
    }

    void runCallback(Args... args)
    {
        if (callback) callback(args...);
    }
};

enum class CompositionState
{
    Begin,
    Update,
    End
};

enum class CandidateListState
{
    Begin,
    Update,
    End
};

/**
 * @details
 * AlphaNumeric - Application receives raw keyboard input
 * Native - Application receives converted text as Commit
 */
enum class InputMode
{
    AlphaNumeric,
    Native
};

struct CandidateListContext
{
    /**
     * @brief Candidate selected
     *
     */
    int32_t                selection;
    /**
     * @brief Candidate strings, in UTF-8
     *
     */
    std::list<std::string> candidates;
};

/**
 * @brief Bounding box of the Application drawn PreEdit text, in window coordinate
 *
 * @details Input method require this to place its CandidateList Window, in non-fullscreen mode
 */
struct PreEditRect
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

/**
 * @brief Application is responsible for drawing the PreEdit
 *
 */
struct PreEditContext
{
    /**
     * @brief Selection start of PreEdit text(inclusive)
     *
     */
    int32_t     selStart;
    /**
     * @brief Selection end of PreEdit text(exclusive)
     *
     */
    int32_t     selEnd;
    /**
     * @brief PreEdit text, UTF-8 encoded
     *
     */
    std::string content;
};

/**
 * @brief Application receives PreEdit here
 *
 * @details
 * CompositionState::Begin, NULL;
 * CompositionState::Update, PreEditContext*;
 * CompositionState::End, NULL;
 */
using PreEditCallbackHolder       = ICallbackHolder<const CompositionState, const PreEditContext*>;
/**
 * @brief Application receives the converted text here
 *
 */
using CommitCallbackHolder        = ICallbackHolder<const std::string>;
/**
 * @brief Application receives CandidateList here
 *
 * @note Only function when fullscreen is set
 * @details
 * CandidateListState::Begin, NULL;
 * CandidateListState::Update, CandidateListContext*;
 * CandidateListState::End, NULL;
 */
using CandidateListCallbackHolder = ICallbackHolder<const CandidateListState, const CandidateListContext*>;

using InputModeCallbackHolder = ICallbackHolder<const InputMode>;

class InputContext
    : public PreEditCallbackHolder
    , public CommitCallbackHolder
    , public CandidateListCallbackHolder
    , public InputModeCallbackHolder
{
  public:
    static const std::string Version;

  public:
    virtual ~InputContext() = default;

  public:
    virtual InputMode   getInputMode()                          = 0;
    virtual void        setPreEditRect(const PreEditRect& rect) = 0;
    virtual PreEditRect getPreEditRect()                        = 0;
    virtual void        setActivated(const bool activated)      = 0;
    virtual bool        getActivated() const                    = 0;
    virtual void        setFullScreen(const bool fullscreen)    = 0;
    virtual bool        getFullScreen() const                   = 0;
};
} // namespace IngameIME
