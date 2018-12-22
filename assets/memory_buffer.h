#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H


#include "fs/file.h"
#include "assets/asset.h"

namespace wage {

  class MemoryBuffer : public Buffer {
    
  public:

    MemoryBuffer() : MemoryBuffer(nullptr, 0) {
    }

    MemoryBuffer(unsigned char* data, size_t length)  {
      fill(data, length);
    }

    MemoryBuffer(Buffer* other) : MemoryBuffer(other->data(), other->length()) {
    }

    ~MemoryBuffer() {
      // printf("Length2: %d\n", length_);
      // if (length_ > 0) {
      //   free(data_);
      // }      
      // TODO: Remve leak!!
    }

    inline void fill(unsigned char* data, size_t length) {
      printf("Length: %d\n", length);
      length_ = length;
      if (length <= 0) {
        data_ = nullptr;
        return;
      }
      data_ = (unsigned char*)malloc(length);
      memcpy(data_, data, length);      
    }

    inline unsigned char* data() {
      return data_;
    }

    inline size_t length() {
      return length_;
    }

  private:

    unsigned char* data_;
    
    size_t length_;
  
  };

}

#endif //MEMORY_BUFFER_H
