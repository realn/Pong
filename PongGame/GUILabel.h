#pragma once

#include <glm/vec4.hpp>

#include "GUIWidget.h"

namespace gui {
  class CLabel
    : public CWidget {
  protected:
    cb::string mText;
    glm::vec4 mTextColor;
    Align mTextAlign;
    glm::vec2 mTextAlignPos;

  public:
    CLabel(cb::string const& id = cb::string(), cb::string const& text = cb::string(),
           glm::vec4 const& textColor = glm::vec4(1.0f), Align const align = Align::Default)
      : CWidget(id), mText(text), mTextColor(textColor), mTextAlign(align) {}
    virtual ~CLabel() {}

    void SetText(cb::string const& text) { mText = text; }
    void SetTextColor(glm::vec4 const& color) { mTextColor = color; }
    void SetTextAlign(Align const align) { mTextAlign = align; }

    cb::string const& GetText() const { return mText; }
    glm::vec4 const& GetTextColor() const { return mTextColor; }
    Align GetTextAlign() const { return mTextAlign; }

    virtual void UpdateRender(CRenderContext const& ctx, glm::vec2 const& spaceSize) override;
    virtual void Render(CRenderContext & ctx, glm::vec2 const & pos) const override;
  };
}
