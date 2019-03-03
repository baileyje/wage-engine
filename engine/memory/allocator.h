#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <memory>
#include <iostream>
#include <inttypes.h>

namespace wage {

  class Allocator {

  public:

    static Allocator* Permanent();

    static Allocator* Temporary();

    static Allocator* Assets();
    
    Allocator(size_t size) : size(size) {
      std::cout << "Acquiring " << size << " bytes of memory" << std::endl;
      memory = malloc(size * sizeof(void*));
      base = memory;
      current = base;
    }

    virtual ~Allocator() {
      free(memory);
    }

    template <typename T, typename... Args>
    T* create(Args... args) {
      auto alloced = allocate(sizeof(T), alignof(T));
      return new(alloced)T(args...);
    }

    void* allocate(size_t size, size_t alignSize = 16) {
      // std::cout << "Attempting to allocate " << size << " bytes of memory" << std::endl;
      if (std::align(alignSize, size, current, this->size)) {
        char* c = (char*)current; // Gross
        if (c + size > ((char*)base) + this->size) {
          std::cout << "Failed to allocate. No free space.";
          return nullptr;
        }
        void* result = current;
        current = c + size;
        return result;
      }
      return nullptr;
    }

    inline void clear() {
      current = base;
    }

  private:
    
    void* memory;

    void* base;

    void* current;
    
    size_t size;

  };

  template <typename T, typename... Args>
  T* make(Args... args) {
    return Allocator::Permanent()->create<T>(args...);
  }

  template <typename T, typename... Args>
  T* makeTemp(Args... args) {
    return Allocator::Temporary()->create<T>(args...);
  }
}

// TODO: bad idea?

#endif //MEMORY_ALLOCATOR_H
