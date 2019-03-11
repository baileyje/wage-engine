#include "assets/fs_manager.h"

#include <memory>

#include "memory/allocator.h"

namespace wage {

  // void FsAssetManager::start(SystemContext* context)  {
  //   // TODO: Pre-load/cache maybe??
  // }

  std::string filePath(Asset::Key key) {
    return "/resources/" + key;
  }

  void FsAssetManager::performLoad(Asset* asset) {
    auto buffer = fileSystem->read(filePath(asset->key()), Allocator::Assets());
    asset->set(std::move(buffer));
  }
}
