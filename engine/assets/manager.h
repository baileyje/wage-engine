#pragma once

#include <string>
#include <mutex>

#include "memory/input_stream.h"
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
      A* load(AS spec, bool useCache = true) {
        if (!useCache) {
          return performLoad<A>(spec);
        }
        auto key = cacheKey(spec);
        auto asset = (A*)cache[key];
        if (asset == nullptr) {
          asset = performLoad<A>(spec);
          cache[key] = asset;
        }
        return asset;
      }

      /**
       * Virtual method setup the input stream for an asset.
       */
      virtual memory::InputStream* assetStream(Asset* asset) = 0;

    private:
      template <typename A = Asset, typename AS = AssetSpec>
      inline A* performLoad(AS spec) {
        auto asset = memory::Allocator::Assets()->create<A>(spec);
        queue.dispatch([this, asset] {
          auto assetStream = this->assetStream(asset);
          if (asset->onLoad(assetStream)) {
            delete assetStream;
          }
          std::unique_lock<std::mutex> lock(mutex);
          loaded.push_back(asset);
        });
        return asset;
      }

      inline std::string cacheKey(AssetSpec spec) {
        return spec.type() + "-" + spec.key();
      }

      async::DispatchQueue queue;

      std::vector<Asset*> loaded;

      std::mutex mutex;

      std::unordered_map<std::string, Asset*> cache;
    };
  } // namespace assets
} // namespace wage
