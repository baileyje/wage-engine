#pragma once

#include <stddef.h>

namespace wage { namespace memory {

  template <typename T, typename IndexType = size_t>
  class Storage {

  public:
    virtual void clear() = 0;

    virtual T& get(IndexType index) = 0;

    virtual T& operator[](IndexType index) {
      return get(index);
    }
  };

} }