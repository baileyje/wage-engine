#pragma once

#include <string>
#include <mutex>

#include "core/core.h"
#include "core/service.h"
#include "async/dispatch_queue.h"
#include "assets/asset.h"

namespace wage { namespace assets {

  class Manager : public core::Service {

  public:
    Manager() : Service("AssetManager"), queue("AssetLoad", 1) {
    }

    void start() {
      core::Core::Instance->onUpdate([&](const core::Frame& frame) {
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
    async::DispatchQueue queue;

    std::vector<Asset*> loaded;

    std::mutex mutex;
  };
} }
