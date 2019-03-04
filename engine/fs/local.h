#ifndef LOCAL_FILE_SYSTEM_H
#define LOCAL_FILE_SYSTEM_H

#include "fs/file_system.h"

namespace wage {
  class LocalFileSystem : public FileSystem {

  public:
    
    LocalFileSystem(std::string base);

    ~LocalFileSystem();

    std::unique_ptr<Buffer> read(std::string path, Allocator* allocator) const override;

  private:

    std::string fullPath(std::string relative) const;

    std::string base;

  };

}

#endif //LOCAL_FILE_SYSTEM_H

