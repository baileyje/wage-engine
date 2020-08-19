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
            asset->state(Asset::AssetState::loaded);
          }
          loaded.clear();
        });
      }

      /**
       * Load an indavidual asset by dispatching the work onto the work queue.
       */
      template <typename A = Asset, typename AS = AssetSpec>
      A* load(AS spec) {
        auto key = cacheKey(spec);
        auto asset = (A*)cache[key];
        if (asset == nullptr) {
          asset = memory::make<A>(spec);
          cache[key] = asset;
          queue.dispatch([this, asset] {
            auto buffer = performLoad(asset);
            asset->onLoad(buffer);
            std::unique_lock<std::mutex> lock(mutex);
            loaded.push_back(asset);
          });
        }
        return asset;        
      }

      /**
       * Virtual method to perform implementation specific load logic.
       */
      virtual memory::Buffer performLoad(Asset* asset) = 0;

    private:
      inline std::string cacheKey(AssetSpec spec) {
        return spec.type() + "-" + spec.key();
      }

      async::DispatchQueue queue;

      std::vector<Asset*> loaded;

      std::mutex mutex;

      std::unordered_map<std::string, Asset*> cache;
    };
  }
}
