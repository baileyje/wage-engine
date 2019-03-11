#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <mutex>

#include "core/core.h"
#include "core/service.h"
#include "async/dispatch_queue.h"
#include "assets/asset.h"

namespace wage {

  class AssetManager : public Service {

  public:
    AssetManager() : Service("AssetManager"), queue("AssetLoad", 1) {
    }

    void start() {
      Core::Instance->onUpdate([&](const Frame& frame) {
        std::unique_lock<std::mutex> lock(mutex);
        for (auto asset : loaded) {
          asset->onLoad();
          asset->loaded(true);
        }
        loaded.clear();
      });
    }

    virtual void load(Asset* asset) {
      queue.dispatch([this, asset] {
        performLoad(asset);
        std::unique_lock<std::mutex> lock(mutex);
        loaded.push_back(asset);
      });
    }

    virtual void performLoad(Asset* asset) = 0;

  private:
    DispatchQueue queue;

    std::vector<Asset*> loaded;

    std::mutex mutex;
  };
}

#endif //ASSET_MANAGER_H
