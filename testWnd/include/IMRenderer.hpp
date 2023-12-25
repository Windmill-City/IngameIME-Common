#pragma once

#include <IngameIME.hpp>
#include <nuklear.h>
#include <string>

class Bounding
{
  public:
    Bounding() = default;

    /**
     * @brief Construct a new Bounding object
     *
     * @param x Horizontal position in pixel
     * @param y Vertical position in pixel
     * @param w Width in pixel
     * @param h Height in pixel
     */
    Bounding(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , w(w)
        , h(h)
    {
    }

    int x, y, w, h;
};

class Padding
{
  public:
    Padding() = default;

    /**
     * @brief Construct a new Padding object
     *
     * @param l Padding left in pixel
     * @param r Padding right in pixel
     * @param t Padding top in pixel
     * @param b Padding bottom in pixel
     */
    Padding(int l, int r, int t, int b)
        : l(l)
        , r(r)
        , t(t)
        , b(b)
    {
    }

    /**
     * @brief Construct a new Padding object
     *
     * @param h Padding left, right in pixel
     * @param v Padding top, bottom in pixel
     */
    Padding(int h, int v)
        : Padding(h, h, v, v)
    {
    }

    /**
     * @brief Construct a new Padding object
     *
     * @param p Padding left, right, top, bottom in pixel
     */
    Padding(int p)
        : Padding(p, p)
    {
    }

    int l, r, t, b;
};

class Color
{
  public:
    Color() = default;

    Color(float r, float g, float b, float a)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }

    Color(float r, float g, float b)
        : Color(r, g, b, 1.f)
    {
    }

    float r, g, b, a;
};

class Renderer
{
  public:
    /**
     * @brief Draw rectangle at position
     *
     * @param bound Bounding of the rectangle
     * @param color Color of the rectangle
     */
    virtual void drawRect(const Bounding& bound, const Color& color) const;

    /**
     * @brief Draw string at position
     *
     * @param x X Pos in pixel
     * @param y Y Pos in pixel
     * @param height Font height in pixel
     * @param text Unicode string
     * @return int Width of the rendered string in pixel
     */
    virtual int drawStr(int x, int y, int height, const Color& color, std::wstring_view text) const;
};

class Widget
{
  public:
    Bounding bound;
    Padding  padding;
    Color    background;
    Color    foreground;

    Widget() = default;

    virtual void setPos(int x, int y)
    {
        bound.x = x;
        bound.y = y;
    }

    virtual void render(const Renderer& renderer) const
    {
        // Background
        renderer.drawRect(bound, background);
    }
};

class TextWidget : public Widget
{
  public:
    std::wstring text;

    TextWidget() = default;

    virtual void render(const Renderer& renderer) const override
    {
        // Background
        Widget::render(renderer);
        // Text
        renderer.drawStr(bound.x, bound.y, 20, foreground, text);
    }
};

/**
 * @brief The IM State Indicator tells the input mode of the active input method when the text edit getting focused,
 * and hide after a few seconds. If the input mode has changed, show a few seconds and hide again.
 *
 */
class IMStateIndicator : public TextWidget
{
  protected:
    bool _show;

    IngameIME::ConversionMode  _conv;
    IngameIME::PunctuationMode _pun;

    IngameIME::InputProcessorCallbackHolder _ipHolder;
    IngameIME::PreEditCallbackHolder        _peHolder;

  public:
    IMStateIndicator(std::shared_ptr<IngameIME::InputContext> im)
    {
        _ipHolder.setCallback(IngameIME::Global::getInstance().setCallback(
            [this, im](IngameIME::InputProcessorState state, const IngameIME::InputProcessorContext& ctx)
            {
                if (im->getActivated())
                {
                    switch (state)
                    {
                    case IngameIME::InputProcessorState::InputModeUpdate:
                        break;
                    case IngameIME::InputProcessorState::FullUpdate:
                        break;
                    }
                }
                this->_ipHolder.runCallback(state, ctx);
            }));
        _peHolder.setCallback(im->comp->IngameIME::PreEditCallbackHolder::setCallback(
            [this](IngameIME::CompositionState state, const IngameIME::PreEditContext* ctx)
            {
                switch (state)
                {
                // Hide indicator when input start
                case IngameIME::CompositionState::Begin:
                    this->hide();
                    break;
                }
                this->_peHolder.runCallback(state, ctx);
            }));
    }

    virtual void show()
    {
        _show = true;
    }

    virtual void hide()
    {
        _show = false;
    }

    virtual void render(const Renderer& renderer) const override
    {
        if (_show) TextWidget::render(renderer);
    }
};