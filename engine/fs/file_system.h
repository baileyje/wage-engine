
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

#include "core/service.h"
#include "fs/file.h"

namespace wage {

  class FileSystem : public Service {

  public:

    FileSystem() : Service("FileSystem") {    
    }

    ~FileSystem() {}

    virtual File* read(std::string path) = 0;

  };

}

#endif //FILE_SYSTEM_H

