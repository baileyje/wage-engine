#include "memory/allocator.h"

namespace wage {
  namespace memory {

    Allocator* Allocator::Permanent() {
      // TODO: Makes these tunable...
      static auto instance = new Allocator("Permanent", 1024 * 1024 * 100);
      return instance;
    }

    Allocator* Allocator::Temporary() {
      // TODO: Makes these tunable...
      static auto instance = new Allocator("Temporary", 1024 * 1024 * 10);
      return instance;
    }

    Allocator* Allocator::Assets() {
      // TODO: Makes these tunable...
      static auto instance = new Allocator("Assets", 1024 * 1024 * 50);
      return instance;
    }

  }
}