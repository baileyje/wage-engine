#pragma once

#include <string>

#include "asset/spec.h"
#include "memory/input_stream.h"

namespace wage {
  namespace asset {

    /**
     * A data backed engine asset like a texture or mesh. Assets have a specific load process
     * and are assumed to need their data loaded from some media format like a filesystem or remote
     * server.
     * 
     * Assets have a specific state transition not loaded to loaded once they are made available. Eventually 
     * the engine may unload them if they are not used after some timeframe.
     */
    class Asset {

    public:
      enum class AssetState {
        // Asset is being tracked, but has not fully loaded
        created,
        // Asset is currently being loaded
        loading,
        // Asset is fully loaded and ready for action
        loaded,
        // The asset data has been clear from memory and will need to be loaded again.
        cleared
      };

      /**
       * Create a asset with a specific type and key. The assumption is these two pieces of information
       * should be enough for a manger to retrieve the underlying data from the storage media.
       */
      Asset(AssetSpec spec) : _state(AssetState::created), _spec(spec) {
      }

      virtual ~Asset() {}

      /**
       * Get the asset spec.
       */
      virtual AssetSpec spec() const {
        return _spec;
      }

      /**
       * Determine if this asset is fully loaded.
       */
      virtual bool loaded() const {
        return _state == AssetState::loaded;
      }

      /**
       * Set the asset state.
       */
      virtual void state(AssetState state) {
        _state = state;
      }

      /**
       * Virtual method called when the asset stream available from the storage media.
       * 
       * Implemantations should set the return value is set to `true` if the loading is complete and 
       * the manager can clean up the input stream and any underlying media resources. On the other 
       * implemantations can return `false` if they plan to use the input stream for an extended period of time.
       * In this case its the asset must be cleaned up by the requestor (or whatever seems cool).
       */
      virtual bool onLoad(memory::InputStream* stream) {
        return true;
      }

    protected:
      AssetState _state;

    private:
      AssetSpec _spec;
    };
  }
}
