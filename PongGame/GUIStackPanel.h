#pragma once

#include "GUIWidget.h"

namespace gui {
  class CStackPanel 
    : public CWidget
  {
  protected:

  public:
    CStackPanel(cb::string const& id)
      : CWidget(id) {}
    virtual ~CStackPanel() {}


  };
}