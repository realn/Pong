#include "stdafx.h"
#include "GameScene.h"

namespace pong {
  CGameScene::CGameScene() {}

  bool CGameScene::Init() {
    return false;
  }

  void CGameScene::Update(const float timeDelta) {
    //  for(auto& controller : mControllers) {
    //    controller->Update(*this, timeDelta);
    //  }
    //  for(auto& paddle : mPaddles) {
    //    paddle->Update(*this, timeDelta);
    //  }
    //  mBall->Update(*this, timeDelta);
  }

  void CGameScene::UpdateRender(const float timeDelta) {
    //  mCanvas->Clear();

    //  for(auto& paddle : mPaddles) {
    //    paddle->UpdateRender(*mCanvas);
    //  }
    //  mBall->UpdateRender(*mCanvas);

    //  mCanvasView->UpdateRender(*mCanvas);
  }

  void CGameScene::Render() {
    //  cb::gl::clearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    //  cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

    //  auto transform = glm::ortho(0.0f, mScreenSize.x, 0.0f, mScreenSize.y);

    //  mCanvasView->Render(transform);
  }

  bool CGameScene::IsFinished() const {
    return false;
  }

  std::unique_ptr<core::CScene> CGameScene::OnCreateScene() {
    return std::unique_ptr<core::CScene>();
  }
}
