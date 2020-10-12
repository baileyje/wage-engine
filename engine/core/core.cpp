#include "core/core.h"

#include "render/renderer.h"

namespace wage {
  namespace core {

    Core* Core::Instance = new Core();

    void Core::postUpdate() {
      for (auto listener : postUpdateListeners) {
        listener(frame());
      }
      if (requestedMode != Mode::none) {
        switch (requestedMode) {
        case Mode::stopped:
          performStop();
          return;
        case Mode::paused:
          performPause();
          return;
        case Mode::unpaused:
          performUnpause();
          return;
        case Mode::resetting:
          performReset();
          return;
        }
      }

      // Signal to the render that we need the next render queue to fill.
      // TODO: What if there is no renderer??
      if (mode != Mode::stopping && mode != Mode::stopped)
        get<render::Renderer>()->swapFrames();
      memory::Allocator::Temporary()->clear();
      // Fire any post update listeners.
    }

  }
}