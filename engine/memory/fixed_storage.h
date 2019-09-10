#pragma once 

#include <cassert>

#include "memory/storage.h"
#include "memory/allocator.h"

namespace wage { namespace memory {

  template <typename T, typename IndexType = size_t>
  class FixedStorage : public Storage<T, IndexType> {

  public:
    FixedStorage(IndexType capacity) : _capacity(capacity) {
      clear();
    }

    virtual void clear() {
      // Something better than this maybe....
      size_t allocSize = sizeof(T) * _capacity; // + sizeof(T) - 1;
      _storage = (T*)Allocator::Permanent()->allocate(allocSize, alignof(T));
    }

    virtual T& get(IndexType index) {
      assert(index < _capacity);
      return _storage[index];
    }

  private:
    IndexType _capacity;

    T* _storage;
  };

} }