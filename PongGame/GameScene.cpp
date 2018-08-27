#include "stdafx.h"

#include <GFXCanvas.h>
#include <GFXCanvasView.h>

#include "Assets.h"
#include "GameObject.h"

#include "GameScene.h"

namespace pong {
  CGameScene::CGameScene(const glm::vec2& screenSize, CAssets& assets) 
    : mScreenSize(screenSize)
  {
    auto textureFileName = L"texture.png"s;
    auto textureSize = glm::uvec2(512);
    auto texture = assets.Textures.Get(textureFileName);
    auto shaderProg = assets.Shaders.Get({ L"font_vs"s, L"font_fs"s });

    shaderProg->SetInLocation(gfx::CCanvasVertex::Inputs);
    shaderProg->Link();

    mCanvas = std::make_unique<gfx::CCanvas>(gfx::CTextureAtlas(textureFileName, textureSize));
    mCanvasView = std::make_unique<gfx::CCanvasView>(shaderProg, texture, texture);
  }

  void CGameScene::AddObject(std::shared_ptr<CGameObject> object) {
    mObjects.push_back(object);
  }

  void CGameScene::Update(const float timeDelta) {
    for (auto& object : mObjects) {
      object->Update(timeDelta);
    }
  }

  void CGameScene::UpdateRender(const float timeDelta) {
      mCanvas->Clear();

      for (auto& object : mObjects) {
        object->UpdateRender(*mCanvas);
      }

      mCanvasView->UpdateRender(*mCanvas);
  }

  void CGameScene::Render() {
      cb::gl::clearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
      cb::gl::clear(cb::gl::ClearBuffer::COLOR | cb::gl::ClearBuffer::DEPTH);

      auto transform = glm::ortho(0.0f, mScreenSize.x, 0.0f, mScreenSize.y);

      mCanvasView->Render(transform);
  }

  bool CGameScene::IsFinished() const {
    return false;
  }

  std::unique_ptr<core::CScene> CGameScene::OnCreateScene() {
    return std::unique_ptr<core::CScene>();
  }
}
