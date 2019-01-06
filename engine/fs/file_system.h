
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

#include "engine/core/system.h"
#include "engine/fs/file.h"

namespace wage {

  class FileSystem : public System {

  public:

    FileSystem() : System("FileSystem") {    
    }

    ~FileSystem() {}

    virtual File* read(std::string path) = 0;

  };

}

#endif //FILE_SYSTEM_H

