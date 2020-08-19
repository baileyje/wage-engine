#pragma once

#include <string>

#include "fs/file_system.h"
#include "assets/asset.h"
#include "assets/manager.h"

namespace wage {
  namespace assets {

    /**
     * Asset manager which uses the filesystem to load indavidual files per asset.
     */
    class FsManager : public Manager {

    public:
      FsManager(fs::FileSystem* fileSystem) : Manager(), fileSystem(fileSystem) {}

      /**
       * Perform an asset load by reading the asset content from the filesystem into a memory buffer.
       */
      memory::Buffer performLoad(Asset* asset) {
        std::cout << "FS PATH: " << filePath(asset->spec()) << "\n";
        return fileSystem->read(filePath(asset->spec()), memory::Allocator::Assets());
      }

    private:
      std::string filePath(AssetSpec assetSpec) {
        return fileSystem->path({"resources", assetSpec.type(), assetSpec.key()});
      }

      fs::FileSystem* fileSystem;
    };
  }
}
