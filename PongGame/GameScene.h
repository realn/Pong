#pragma once

#include <CoreScene.h>

namespace pong {
  class CGameScene
    : public core::CScene {
  public:
    CGameScene();

    bool Init();

    // Inherited via CScene
    virtual void Update(const float timeDelta) override;
    virtual void UpdateRender(const float timeDelta) override;
    virtual void Render() override;

    virtual bool IsFinished() const override;
    virtual std::unique_ptr<core::CScene> OnCreateScene() override;
  };
}