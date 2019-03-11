
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

#include "core/service.h"
#include "memory/buffer.h"
#include "memory/allocator.h"

#include "fs/file.h"

namespace wage {

  class FileSystem : public Service {

  public:
    FileSystem() : Service("FileSystem") {
    }

    ~FileSystem() {}

    inline File get(std::string path) const {
      return File(path, this);
    }

    virtual std::unique_ptr<Buffer> read(std::string path, Allocator* allocator) const = 0;
  };
}

#endif //FILE_SYSTEM_H
