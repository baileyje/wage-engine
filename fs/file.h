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
      size_ = file.tellg();
      printf("Path: %s\n", path.c_str());
      printf("Length: %zu\n", size_);      
      file.seekg (0, std::ios::beg);
      data_ = (unsigned char*)Allocator::Assets()->allocate(size_ + 15);
      file.read ((char*)data_, size_);
      file.close();
    }

    inline unsigned char* data() {
      return data_;
    }

    inline size_t length() {
      return size_;
    }

  private:

    unsigned char* data_;

    size_t size_;

  };

}

#endif //FILE_SYSTEM_FILE_H