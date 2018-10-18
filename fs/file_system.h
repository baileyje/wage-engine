
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <string>

class FileSystem {

public:

  virtual std::string* read(std::string path) = 0;

};


#endif //FILE_SYSTEM_H

