#pragma once

#include <string>

#include "fs/file_system.h"
#include "assets/asset.h"
#include "assets/manager.h"

namespace wage { namespace assets {

  class FsManager : public Manager {

  public:
    FsManager(fs::FileSystem* fileSystem) : Manager(), fileSystem(fileSystem) {
    }

    virtual void performLoad(Asset* asset);

  private:
    fs::FileSystem* fileSystem;
  };
} }
