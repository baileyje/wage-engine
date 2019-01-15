#ifndef POOL_STORAGE_H
#define POOL_STORAGE_H

#include <stddef.h>

namespace wage {

  template <typename T, typename IndexType = size_t>
  class PoolStorage {

  public:
    
    virtual void clear() = 0;

    // virtual IndexType next() = 0;

    virtual void put(IndexType index, T value) = 0; 

    virtual T& get(IndexType index) = 0;

    virtual T& operator[](IndexType index) {
      return get(index);
    }

  };

}

#endif //POOL_STORAGE_H