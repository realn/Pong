#pragma once

namespace core {
  class CScene {
  public:
    virtual ~CScene() {}

    virtual void Update(const float timeDelta) = 0;
    virtual void UpdateRender(const float timeDelta) = 0;
    virtual void Render() = 0;

    virtual bool IsFinished() const = 0;
    virtual std::unique_ptr<CScene> OnCreateScene() = 0;
  };
}