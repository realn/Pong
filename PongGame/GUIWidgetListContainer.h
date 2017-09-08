#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include "GUIConsts.h"

namespace gui {
  class CWidget;

  class CWidgetListItem {
  public:
    glm::vec2 Pos;
    std::unique_ptr<CWidget> Widget;
    Align Align;
  };

  class CWidgetListContainer {
  protected:
    std::vector<CWidgetListItem> mItems;

  public:
    virtual ~CWidgetListContainer();

    bool empty() const { return mItems.empty(); }
    size_t size() const { return mItems.size(); }

    virtual CWidget* FindWidgetById(cb::string const& id) = 0;
    virtual const CWidget* FindWidgetById(cb::string const& id) const = 0;

    
  };
}
