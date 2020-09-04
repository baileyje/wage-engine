#include "core/core.h"

#include "render/renderer.h"

namespace wage {
  namespace core {

    Core* Core::Instance = memory::make<Core>();

    void Core::postUpdate() {
      get<render::Renderer>()->swapFrames();
      memory::Allocator::Temporary()->clear();
    }

  }
}