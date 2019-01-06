#include "engine/fs/local.h"

#include <fstream>
#include "engine/memory/allocator.h"

namespace wage {

  LocalFileSystem::LocalFileSystem(std::string base) : base(base) {
  }

  LocalFileSystem::~LocalFileSystem() {  
  }

  File* LocalFileSystem::read(std::string path) {
    return make<File>(base + "/" + path);
  }

}