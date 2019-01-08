#include "memory/allocator.h"

namespace wage {

  Allocator* Allocator::Permanent() {
    static auto instance = new Allocator(1024*1024* 50);
    return instance;
  }
  

  Allocator* Allocator::Temporary() {
    static auto instance = new Allocator(1024*1024*50);
    return instance;
  }

  Allocator* Allocator::Assets() {
    static auto instance = new Allocator(1024*1024*100);
    return instance;
  }

}