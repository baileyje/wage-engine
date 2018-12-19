#ifndef FS_ASSET_MANAGER_H
#define FS_ASSET_MANAGER_H

#include <string>

#include "core/system.h"
#include "fs/file_system.h"
#include "assets/asset.h"
#include "assets/manager.h"

namespace wage {

  class FsAssetManager : public AssetManager {

  public:

    FsAssetManager(FileSystem* fileSystem) : AssetManager(), fileSystem(fileSystem) {
    }

    LIFECYCLE_FUNC(start)
    
    virtual void load(Asset* asset);

  private:
    
    FileSystem* fileSystem;
  
  };

}

#endif //FS_ASSET_MANAGER_H
