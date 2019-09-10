#include "assets/fs_manager.h"

#include <memory>

#include "memory/allocator.h"

namespace wage { namespace assets {

  std::string filePath(Asset::Key key) {
    return "/resources/" + key;
  }

  void FsManager::performLoad(Asset* asset) {
    auto buffer = fileSystem->read(filePath(asset->key()), memory::Allocator::Assets());
    asset->set(std::move(buffer));
  }
} }
