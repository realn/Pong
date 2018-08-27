#include "stdafx.h"

#include "GamePaddleController.h"

#include "GameController.h"

namespace pong {
  std::shared_ptr<IGameController> IGameController::Create(GameControllerType const type,
                                                           std::shared_ptr<CGameObject> target) {
    switch(type) {
    case GameControllerType::Mouse:  
      return std::make_shared<CGamePaddleLocalController>(target, CGamePaddleLocalControllerConfig());
    //case GameControllerType::Keyboard:
    //  return std::make_shared<CGamePaddleMouseController>(target);
    default:
      throw std::exception("Unknown game controller type.");
    }
  }
}
