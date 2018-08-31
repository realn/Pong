#pragma once

namespace pong {
  class CGameSceneProcess {
  public:
    CGameSceneProcess() {}
    virtual ~CGameSceneProcess() {}

    virtual void Update(const float timeDelta) = 0;
  };
}