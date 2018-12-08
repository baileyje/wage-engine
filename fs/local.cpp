#include "fs/local.h"

#include <fstream>

namespace wage {

  LocalFileSystem::LocalFileSystem(std::string base) : base(base) {
  }

  LocalFileSystem::~LocalFileSystem() {  
  }

  File* LocalFileSystem::read(std::string path) {
    std::string fullPath = base + "/" + path;
    std::ifstream stream(fullPath, std::fstream::binary);    
    return new File(stream);
  }

}