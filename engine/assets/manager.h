#pragma once

#include <string>
#include <mutex>

#include "core/core.h"
#include "core/service.h"
#include "async/dispatch_queue.h"
#include "assets/asset.h"

namespace wage {
  namespace assets {

    /**
     * Manages the loading and unloading of assets for the engine. It is upto the implementations to decide how the 
     * actual loading is done.
     */
    class Manager : public core::Service {

    public:
      Manager() : Service("AssetManager"), queue("AssetLoad", 2) {
      }

      /**
       * Start the asset manager and register to be updated on each engine loop.
       */
      void start() {
        core::Core::Instance->onUpdate([&](const core::Frame& frame) {
          std::unique_lock<std::mutex> lock(mutex);
          for (auto asset : loaded) {
            asset->loaded(true);
          }
          loaded.clear();
        });
      }

      /**
       * Load an indavidual asset by dispatching the work onto the work queue.
       */
      virtual void load(Asset* asset) {
        queue.dispatch([this, asset] {
          performLoad(asset);
          asset->onLoad();
          std::unique_lock<std::mutex> lock(mutex);
          loaded.push_back(asset);
        });
      }

      /**
       * Virtual method to perform implementation specific load logic.
       */
      virtual void performLoad(Asset* asset) = 0;

    private:
      async::DispatchQueue queue;

      std::vector<Asset*> loaded;

      std::mutex mutex;
    };
  }
}
