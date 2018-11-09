
#ifndef LOCAL_FILE_SYSTEM_H
#define LOCAL_FILE_SYSTEM_H

#include "fs/file_system.h"

class LocalFileSystem : public FileSystem {

public:
  
  LocalFileSystem(std::string base);

  ~LocalFileSystem();

  const char* read(std::string path);

private:

  std::string base;

};


#endif //LOCAL_FILE_SYSTEM_H

