#include "core/core.h"

#include "render/renderer.h"

namespace wage {
  namespace core {

    Core* Core::Instance = new Core();

    void Core::postUpdate() {
      // Signal to the render that we need the next render queue to fill.
      // TODO: What if there is no renderer??
      get<render::Renderer>()->swapFrames();
      memory::Allocator::Temporary()->clear();
      if (mode == Mode::resetting) {
        memory::Allocator::Permanent()->clear();
        for (auto service : services) {
          Logger::info("Resetting ", service->name().c_str());
          service->reset();
        }
        // TODO: Reload scene
        mode = Mode::running;
      }
    }

  }
}