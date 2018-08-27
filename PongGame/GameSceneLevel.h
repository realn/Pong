#pragma once

#include <CoreScene.h>

namespace pong {
  class CGameSceneMain
    : public core::CScene {
  public:
    CGameSceneMain();

    bool Init();

    // Inherited via CScene
    virtual void Update(const float timeDelta) override;
    virtual void UpdateRender(const float timeDelta) override;
    virtual void Render() override;

    virtual bool IsFinished() const override;
    virtual std::unique_ptr<core::CScene> CreateScene() override;
  };
}