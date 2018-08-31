#pragma once

#include <vector>

#include <CoreScene.h>
#include <GFXFwd.h>

namespace pong {
  class CAssets;
  class CGameObject;
  class CGameSceneProcess;

  class CGameScene
    : public core::CScene {
  protected:
    using ObjectsT = std::vector<std::shared_ptr<CGameObject>>;
    using ProcessesT = std::vector<std::shared_ptr<CGameSceneProcess>>;

    glm::vec2 mScreenSize;
    ObjectsT mObjects;
    ProcessesT mProcesses;
    std::unique_ptr<gfx::CCanvas> mCanvas;
    std::unique_ptr<gfx::CCanvasView> mCanvasView;

  public:
    CGameScene(const glm::vec2& screenSize, CAssets& assets);

    void AddObject(std::shared_ptr<CGameObject> object);
    void AddProcess(std::shared_ptr<CGameSceneProcess> process);

    // Inherited via CScene
    virtual void Update(const float timeDelta) override;
    virtual void UpdateRender(const float timeDelta) override;
    virtual void Render() override;

    virtual bool IsFinished() const override;
    virtual std::unique_ptr<core::CScene> OnCreateScene() override;
  };
}