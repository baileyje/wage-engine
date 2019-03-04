#include "fs/file.h"

#include "fs/file_system.h"

namespace wage {
  
  std::unique_ptr<Buffer> File::read(Allocator* allocator) const {
    return _fileSystem->read(_path, allocator);
  }

}