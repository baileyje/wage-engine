#pragma once

#include <cstdint>

#include "memory/buffer.h"
#include "memory/common.h"

namespace wage {
  namespace memory {

    class InputStream {
    public:
      virtual ~InputStream() {}

      /**
       * Read a block of data update the provided size from the underlying storage and store it in the 
       * data pointer provided. Return the amount of data actually read.
       */
      virtual size_t read(Byte* data, size_t size) = 0;

      /**
       * Seek the read pointer to the position provided.
       */
      virtual size_t seek(size_t position) = 0;

      /**
       * Get the current read postion.
       */
      virtual size_t tell() = 0;

      /**
       * Get the size of the content that can be read.
       */
      virtual size_t size() = 0;

      /**
       * Explicitly close the input stream..
       */
      virtual void close() = 0;

      /**
       * Read the entire input stream into a memory buffer back by the provided allocator.
       */
      virtual memory::Buffer read(memory::Allocator* allocator) {
        memory::Buffer buffer;
        seek(0);
        auto bufferSize = size();
        buffer.allocate(bufferSize, allocator);
        read(buffer.data(), bufferSize);
        seek(0);
        return buffer;
      };
    };

  }
}