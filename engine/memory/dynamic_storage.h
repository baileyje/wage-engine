#ifndef DYNAMIC_STORAGE_H
#define DYNAMIC_STORAGE_H

#include <cassert>

#include "memory/pool_storage.h"

// #include "memory/allocator.h"
// #include "memory/reference.h"

namespace wage {

  template <typename T, typename IndexType = size_t>
  class DynamicStorage : public PoolStorage<T, IndexType> {

  public:
  
    DynamicStorage(int chunkSize = 100) : _chunkSize(chunkSize) { /* _currentSize(0)*/
      clear();
    }

    void clear() {
      
    }

    virtual IndexType next() {
      return 0;
    }

    virtual void put(IndexType index, T value) {

    }

    T& get(IndexType index) {
      int chunkIndex = index / _chunkSize;
      int itemIndex = index % _chunkSize;
      printf("Lookup:  %d -> %d\n", chunkIndex, itemIndex);
      // return index < currentSize ? &storage[index].item : nullptr;
      auto res = new T;
      return *res;
    }

    // const T* get(Index index) const {
    //   // return index < currentSize ? &storage[index].item : nullptr;
    //   return nullptr;
    // }

    // Node* storage;
    
    int _chunkSize;

    // int _currentSize;

  };

}

#endif //DYNAMIC_STORAGE_H