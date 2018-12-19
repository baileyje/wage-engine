#include "assets/fs_manager.h"

#include <memory>

#include "assets/fs_buffer.h"

namespace wage {

  void FsAssetManager::start(SystemContext* context)  {
    // TODO: Pre-load/cache maybe??
  }

  std::string filePath(Asset::Key key) {
    return "/resources/" + key;
  }

  void FsAssetManager::load(Asset* asset) {
    asset->set(new FileBuffer(fileSystem->read(filePath(asset->key()))));
  }

}



