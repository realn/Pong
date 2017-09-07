#pragma once

#include <memory>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace gfx {
  class CCanvas;
  class CFont;
}

namespace gui {
  class CWidget;
  struct CRenderContext;

  class CScreen {
  private:
    std::unique_ptr<CWidget> mContent;
    glm::vec2 mSize;

  public:
    CScreen(glm::vec2 const& size = glm::vec2()) : mSize(size) {}
    ~CScreen();

    void SetContent(std::unique_ptr<CWidget> content) { mContent = std::move(content); }
    void SetSize(glm::vec2 const& size) { mSize = size; }

    CWidget* GetContent() const { return mContent.get(); }
    glm::vec2 GetSize() const { return mSize; }

    std::unique_ptr<CWidget> ReleaseContent() { return std::move(mContent); }

    void Update(float timeDelta);
    void UpdateRender(gfx::CFont const& font, gfx::CCanvas& canvas);

    CWidget* FindWidgetById(cb::string const& id);
    const CWidget* FindWidgetById(cb::string const& id) const;

    template<typename _Type>
    _Type* FindById(cb::string const& id) {
      return dynamic_cast<_Type*>(FindWidgetById(id));
    }
    template<typename _Type>
    const _Type* FindById(cb::string const& id) const {
      return dynamic_cast<const _Type*>(FindWidgetById(id));
    }
  };
}