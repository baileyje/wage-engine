#include "memory/allocator.h"

namespace wage {

  Allocator* Allocator::Permanent() {
    static auto instance = new Allocator("Permanent", 1024*1024*50);
    return instance;
  }

  Allocator* Allocator::Temporary() {
    static auto instance = new Allocator("Temporary", 1024*1024*50);
    return instance;
  }

  Allocator* Allocator::Assets() {
    static auto instance = new Allocator("Assets", 1024*1024*50);
    return instance;
  }

}