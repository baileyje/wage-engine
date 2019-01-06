#ifndef FS_ASSET_MANAGER_H
#define FS_ASSET_MANAGER_H

#include <string>

#include "engine/core/system.h"
#include "engine/fs/file_system.h"
#include "engine/assets/asset.h"
#include "engine/assets/manager.h"

namespace wage {

  class FsAssetManager : public AssetManager {

  public:

    FsAssetManager(FileSystem* fileSystem) : AssetManager(), fileSystem(fileSystem) {
    }

    LIFECYCLE_FUNC(start)
    
    virtual void performLoad(Asset* asset);
    
  private:
    
    FileSystem* fileSystem;

  };

}

#endif //FS_ASSET_MANAGER_H
