#pragma once

#include <string>

#include "core/service.h"
#include "memory/buffer.h"
#include "memory/allocator.h"

#include "fs/file.h"

namespace wage { namespace fs {

  class FileSystem : public core::Service, public File::ContentProvider {

  public:
    FileSystem() : Service("FileSystem") {
    }

    ~FileSystem() {}

    inline File get(std::string path) const {
      return File(path, this);
    }

    virtual std::unique_ptr<memory::Buffer> read(std::string path, memory::Allocator* allocator) const = 0;
  };

} }
