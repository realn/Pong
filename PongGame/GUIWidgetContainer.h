#pragma once

#include <memory>
#include <glm/vec4.hpp>

namespace gui {
  class CWidget;

  class CWidgetContainer {
  protected:
    std::unique_ptr<CWidget> mContent;
    glm::vec4 mContentMargin;
    Align mContentAlign;

  public:
    CWidgetContainer(glm::vec4 const& contentMargin, Align const contentAlign)
      : mContentMargin(contentMargin), mContentAlign(contentAlign) {}
    virtual ~CWidgetContainer();

    void SetContent(std::unique_ptr<CWidget> content) { mContent = std::move(content); }
    void SetContentMargin(glm::vec4 const& margin) { mContentMargin = margin; }
    void SetContentAlign(Align const align) { mContentAlign = align; }

    CWidget* GetContent() { return mContent.get(); }
    const CWidget* GetContent() const { return mContent.get(); }
    glm::vec4 const& GetContentMargin() const { return mContentMargin; }
    Align GetContentAlign() const { return mContentAlign; }

    std::unique_ptr<CWidget> ReleaseContent() { return std::move(mContent); }

    virtual CWidget* FindWidgetById(cb::string const& id) = 0;
    virtual const CWidget* FindWidgetById(cb::string const& id) const = 0;
  };
}
