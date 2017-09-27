#pragma once

#include <memory>
#include <map>
#include <CBStr/Defines.h>

#include "CoreFont.h"

namespace core {
  template<typename _Type>
  class CRepository {
  private:
    cb::string mAssetDir;
    cb::string mAssetExt;
    std::map<cb::string, std::weak_ptr<_Type>> mRepo;

  public:
    CRepository(cb::string const& assetDir, cb::string const& assetExt)
      : mAssetDir(assetDir), mAssetExt(assetExt) {}
    virtual ~CRepository() = default;

    std::shared_ptr<_Type> Get(cb::string const& name);
    std::shared_ptr<_Type> Load(cb::string const& name) = 0;

  protected:
    virtual cb::string GetAssetPath(cb::string const& name) const;
    virtual std::shared_ptr<_Type> LoadAsset(cb::string const& name);
  };

  template<typename _Type>
  inline std::shared_ptr<_Type> CRepository<_Type>::Get(cb::string const & name) {
    auto it = mRepo.find(name);
    if(it == mRepo.end()) {
      return LoadAsset(name);
    }
    auto ptr = it->secont.lock();
    if(!ptr) {
      return LoadAsset(name);
    }
    return ptr;
  }
}