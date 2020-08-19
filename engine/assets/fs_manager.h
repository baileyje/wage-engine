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
      void performLoad(Asset* asset) {
        auto buffer = fileSystem->read(filePath(asset), memory::Allocator::Assets());
        asset->set(buffer);
      }

    private:
      std::string filePath(Asset* asset) {
        return fileSystem->path({"resources", asset->type(), asset->key()});
      }

      fs::FileSystem* fileSystem;
    };
  }
}
