#pragma once

#include <memory>

#include "GUIRect.h"

namespace gui {
  class CPanel 
    : public CRect
  {
  protected:
    glm::vec4 mContentMargin;
    std::unique_ptr<CWidget> mContent;
    Align mContentAlign;
    glm::vec2 mContentPos;

  public:
    CPanel(cb::string const& id, 
           glm::vec4 const& backColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
           glm::vec4 const& contentMargin = glm::vec4(0.0f),
           Align const contentAlign = Align::Default)
      : CRect(id, backColor), mContentMargin(contentMargin), mContentAlign(contentAlign) {}
    virtual ~CPanel() {}

    void SetContent(std::unique_ptr<CWidget> widget) { mContent = std::move(widget); }
    void SetContentMargin(glm::vec4 const& margin) { mContentMargin = margin; }
    void SetContentAlign(Align const align) { mContentAlign = align; }

    CWidget& GetContent() const { return *mContent; }
    glm::vec4 const& GetContentMargin() const { return mContentMargin; }
    Align GetContentAlign() const { return mContentAlign; }

    std::unique_ptr<CWidget> ReleaseContent() { return std::move(mContent); }

    virtual void UpdateRender(CRenderContext const& ctx, glm::vec2 const& spaceSize);
    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const;
  };
}
