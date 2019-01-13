#ifndef FILE_SYSTEM_FILE_H
#define FILE_SYSTEM_FILE_H

#include <iostream>
#include <fstream>

#include "memory/allocator.h"

namespace wage {

  class File {

  public:

    File(std::string path)  {
      std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
      _size = file.tellg();
      printf("Path: %s\n", path.c_str());
      printf("Length: %zu\n", _size);      
      file.seekg (0, std::ios::beg);
      _data = (unsigned char*)Allocator::Assets()->allocate(_size + 15);
      file.read ((char*)_data, _size);
      file.close();
    }

    inline unsigned char* data() {
      return _data;
    }

    inline size_t length() {
      return _size;
    }

  private:

    unsigned char* _data;

    size_t _size;

  };

}

#endif //FILE_SYSTEM_FILE_H