#pragma once

#include <streambuf>

#include "memory/common.h"

namespace wage {
  namespace memory {

    class BufferStream : public std::basic_streambuf<char> {

    public:
      BufferStream(Byte* data, size_t size) {
        setg((char*)data, (char*)data, (char*)data + size);
      }
    };
  }
}
