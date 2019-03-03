#ifndef MEMORY_BUFFER_H
#define MEMORY_BUFFER_H


#include "fs/file.h"
#include "assets/asset.h"

namespace wage {

  class MemoryBuffer : public Buffer {
    
  public:

    MemoryBuffer() : MemoryBuffer(nullptr, 0) {
    }

    MemoryBuffer(const unsigned char* data, size_t length)  {
      fill(data, length);
    }

    MemoryBuffer(Buffer* other) : MemoryBuffer(other->data(), other->length()) {
    }

    ~MemoryBuffer() {      
      // TODO: Remove leak!!
    }

    inline void fill(const unsigned char* data, size_t length) {
      _length = length;
      if (length <= 0) {
        _data = nullptr;
        return;
      }
      _data = (unsigned char*)malloc(length);
      memcpy(_data, data, length);      
    }

    inline const unsigned char* data() const {
      return _data;
    }

    inline size_t length() const {
      return _length;
    }

  private:

    unsigned char* _data;
    
    size_t _length;
  
  };

}

#endif //MEMORY_BUFFER_H
