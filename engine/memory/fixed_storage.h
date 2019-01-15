#ifndef FIXED_STORAGE_H
#define FIXED_STORAGE_H

#include <cassert>

#include "memory/pool_storage.h"


namespace wage {

  template <typename T, typename IndexType = size_t>
  class FixedStorage : public PoolStorage<T, IndexType> {

  public:

    FixedStorage(IndexType capacity) : _capacity(capacity) {
      clear();
    }
    
    virtual void clear() {
      size_t allocSize = sizeof(T) * _capacity; // + sizeof(T) - 1;
      _storage = (T*)Allocator::Permanent()->allocate(allocSize, alignof(T));    
    }

    virtual IndexType next() {
      // return _size;
      return 0;
    }

    virtual void put(IndexType index, T value) {
      assert(index < _capacity);
      // _storage[index] = value;
    } 

    virtual T& get(IndexType index) {
      assert(index < _capacity);
      return _storage[index];
    }

  private:

    IndexType _capacity;

    T* _storage;

  };

}

#endif //FIXED_STORAGE_H