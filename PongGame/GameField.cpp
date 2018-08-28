#include "stdafx.h"

#include <CBGL/Buffer.h>
#include <CBGL/Program.h>
#include <GFXConsts.h>
#include <GFXCanvas.h>

#include "Consts.h"
#include "GameField.h"

namespace pong {
  CGameField::CGameField() {}

  CGameField::~CGameField() {}

  void CGameField::Update(float const timeDelta) {}

  void CGameField::UpdateRender(gfx::CCanvas& canvas) {
    canvas.DrawRect(GetBRect(), mColor);
  }
}
