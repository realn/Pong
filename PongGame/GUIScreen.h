#pragma once

#include <memory>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include "GUIWidgetContainer.h"

namespace gfx {
  class CCanvas;
  class CFont;
  class CTextureAtlas;
}

namespace gui {
  class CWidget;
  struct CRenderContext;

  class CScreen 
    : public CWidgetContainer
  {
  private:
    glm::vec2 mSize;

  public:
    CScreen(glm::vec2 const& size = glm::vec2(), 
            glm::vec4 const& contentMargin = glm::vec4(), 
            Align const contentAlign = Align::Default);
    virtual ~CScreen();

    void SetSize(glm::vec2 const& size) { mSize = size; }

    glm::vec2 GetSize() const { return mSize; }

    void Update(float const timeDelta);
    void UpdateRender(gfx::CCanvas& canvas, gfx::CFont const& font);

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