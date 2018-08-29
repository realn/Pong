#pragma once

#include <CBGL/Buffer.h>

#include "GameMovingObject.h"

namespace pong {
  class CGameField;

  class CGameBall
    : public CGameMovingObject {
  private:
    std::shared_ptr<CGameField> mField;

  public:
    CGameBall();
    ~CGameBall();

    void SetField(std::shared_ptr<CGameField> field);

    std::shared_ptr<CGameField> GetField() const;

    virtual void Update(float const timeDelta) override;

    virtual void UpdateRender(gfx::CCanvas& canvas) override;
  };
}