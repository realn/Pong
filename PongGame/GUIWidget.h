#pragma once

#include <CBStr/Defines.h>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace gui {
  ENUM_FLAG(Align) {
    Left = 0x1,
      Right = 0x2,
      Top = 0x4,
      Bottom = 0x8,
      Center = (Left | Right),
      Middle = (Top | Bottom),
      MidCenter = (Center | Middle),
      Default = (Top | Left),
  };

  enum class Orientation {
    Horizontal = 0,
    Vertical = 1,
  };

  struct CRenderContext;

  class CWidget {
  private:
    cb::string mId;

  protected:
    glm::vec2 mSize;
    glm::vec2 mFixedSize;

  public:
    CWidget(cb::string const& id) : mId(id) {}
    virtual ~CWidget() {}

    void SetFixedSize(glm::vec2 const& size) { mFixedSize = size; }

    cb::string const& GetId() const { return mId; }
    glm::vec2 const& GetSize() const { return mSize; }
    glm::vec2 const& GetFixedSize() const { return mFixedSize; }

    virtual void Update(float const timeDelta) {}
    virtual void UpdateRender(CRenderContext const& ctx, glm::vec2 const& spaceSize);
    virtual void Render(CRenderContext& ctx, glm::vec2 const& pos) const = 0;

    virtual CWidget* FindWidgetById(cb::string const& id) { return (id == mId) ? this : nullptr; }
    virtual const CWidget* FindWidgetById(cb::string const& id) const { return (id == mId) ? this : nullptr; }

    static cb::string NoId;

  protected:
    static glm::vec2 GetAlignedPos(glm::vec2 const& size, glm::vec2 const& spaceSize, Align const align);
    static glm::vec2 GetMarginPos(glm::vec4 const& margin);
    static glm::vec2 GetMarginSize(glm::vec2 const& size, glm::vec4 const& margin);
  };
}
