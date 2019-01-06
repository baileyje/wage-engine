#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>

#include "engine/core/system.h"
#include "engine/async/dispatch_queue.h"
#include "engine/assets/asset.h"

namespace wage {

  class AssetManager : public System {

  public:

    AssetManager() : System("AssetManager"), queue("AssetLoad", 1) {
    }
    
    virtual void load(Asset* asset) {
      queue.dispatch([this,asset]{
        performLoad(asset);
      });      
    }

    virtual void performLoad(Asset* asset) = 0;

  private:

    DispatchQueue queue;

  };

}

#endif //ASSET_MANAGER_H
