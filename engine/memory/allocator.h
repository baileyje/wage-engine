#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <memory>
#include <iostream>
#include <inttypes.h>

#include "core/logger.h"

namespace wage {

  class Allocator {

  public:
    static Allocator* Permanent();

    static Allocator* Temporary();

    static Allocator* Assets();

    Allocator(std::string name, size_t size) : name(name), size(size) {
      Logger::debug("Acquiring ", size, " bytes of memory for allocator: ", name);
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
      return new (alloced) T(args...);
    }

    void* allocate(size_t size, size_t alignSize = 16) {
      Logger::debug("Attempting to allocate ", size, " bytes of memory for allocator: ", name);
      if (std::align(alignSize, size, current, this->size)) {
        char* c = (char*)current; // Gross
        if (c + size > ((char*)base) + this->size) {
          Logger::error("Failed to allocate. No free space.");
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
    std::string name;

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

#endif //MEMORY_ALLOCATOR_H
