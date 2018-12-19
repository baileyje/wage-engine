#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>

#include "core/system.h"
#include "assets/asset.h"

namespace wage {

  class AssetManager : public System {

  public:

    AssetManager() : System("AssetManager") {
    }
    
    virtual void load(Asset* asset) = 0;

  };

}

#endif //ASSET_MANAGER_H
