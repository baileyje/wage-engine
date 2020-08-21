#pragma once

#include "memory/allocator.h"
#include "memory/common.h"

namespace wage {
  namespace memory {

    class Buffer : public std::basic_streambuf<char> {

    public:
      Buffer() : _data(nullptr), _length(0) {}

      inline Byte* data() { return _data; }

      inline const Byte* data() const { return _data; }

      inline size_t length() const { return _length; }

      inline void allocate(size_t length, Allocator* allocator = nullptr) {
        if (allocator) {
          _data = (Byte*)allocator->allocate(length);
        } else {
          _data = (Byte*)Allocator::Permanent()->allocate(length);
        }
        _length = length;
        // Setup as stream buffer
        setg((char*)_data, (char*)_data, (char*)_data + length);
      }

      inline void fill(const Byte* data, size_t length, Allocator* allocator = nullptr) {
        if (length <= 0) {
          _data = nullptr;
          return;
        }
        allocate(length, allocator);
        memcpy(_data, data, length);
      }

    private:
      Byte* _data;

      size_t _length;
    };
  }
}
