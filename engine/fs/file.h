#ifndef FILE_SYSTEM_FILE_H
#define FILE_SYSTEM_FILE_H

#include <memory>

#include "memory/buffer.h"

namespace wage {

  class FileSystem;

  class File {

  public:

    File(std::string path, const FileSystem* fileSystem) : _path(path), _fileSystem(fileSystem) {
    }

    inline std::string path() const {
      return _path;
    }

    std::unique_ptr<Buffer> read(Allocator* allocator = nullptr) const;

  private:

    std::string _path;

    const FileSystem* _fileSystem;

  };

}

#endif //FILE_SYSTEM_FILE_H