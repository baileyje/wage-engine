
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

#include "core/system.h"

class FileSystem : public System {

public:

  FileSystem() : System("FileSystem") {    
  }

  ~FileSystem() {}

  virtual const char* read(std::string path) = 0;

};

#endif //FILE_SYSTEM_H

