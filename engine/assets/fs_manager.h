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

      inline memory::InputStream* assetStream(Asset* asset) {
        return fileSystem->readStream(filePath(asset->spec()));
      }

    private:
      fs::File::Path filePath(AssetSpec assetSpec) {
        return {{"resources", assetSpec.type(), assetSpec.key()}};
      }

      fs::FileSystem* fileSystem;
    };
  }
}
