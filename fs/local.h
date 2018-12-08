
#ifndef LOCAL_FILE_SYSTEM_H
#define LOCAL_FILE_SYSTEM_H

#include "fs/file_system.h"

namespace wage {
  class LocalFileSystem : public FileSystem {

  public:
    
    LocalFileSystem(std::string base);

    ~LocalFileSystem();

    File* read(std::string path);

  private:

    std::string base;

  };

}

#endif //LOCAL_FILE_SYSTEM_H

