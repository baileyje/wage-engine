#include "engine/assets/fs_manager.h"

#include <memory>

#include "engine/assets/fs_buffer.h"
#include "engine/memory/allocator.h"

namespace wage {

  void FsAssetManager::start(SystemContext* context)  {
    // TODO: Pre-load/cache maybe??
  }

  std::string filePath(Asset::Key key) {
    return "/resources/" + key;
  }

  void FsAssetManager::performLoad(Asset* asset) {
    auto file = fileSystem->read(filePath(asset->key()));
    asset->set(make<FileBuffer>(file));
  }

}



