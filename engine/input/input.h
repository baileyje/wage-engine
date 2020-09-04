#pragma once

#include <unordered_set>
#include <mutex>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"
#include "core/core.h"
#include "platform/platform.h"

#include "input/key.h"
#include "input/mouse_button.h"

namespace wage {
  namespace input {

    /**
     * System responsible for exposing inpput events/state to other service or systems at runtime. This is a pure
     * virtual service and requires a concrete implementation to be added to the engine to function.
     */
    class Input : public core::Service {

    public:
      Input() : Service("Input"), window(nullptr), messaging(nullptr) {}

      ~Input() {}

      /**
       * Start the input system by gaining access to the platform to grab input events from.
       */
      void start() {
        messaging = core::Core::Instance->get<messaging::Messaging>();
        auto platform = core::Core::Instance->get<platform::Platform>();
        window = platform->window();
        core::Core::Instance->onInput([&](const core::Frame& frame) {
          clear();
        });
      }

      void clear() {
        framePress.clear();
        frameRelease.clear();
        frameMousePress.clear();
        frameMouseRelease.clear();
      }

      /**
       * Interface for determining if a specific key is currently pressed. This will return true until it is let go. 
       * There is not state management behind this answer.
       */
      virtual bool isPressed(Key key) const = 0;

      /**
       * Interface for determining if a specific mouse button is currently pressed. This will return true until it is let go. 
       * There is not state management behind this answer.
       */
      virtual bool isPressed(MouseButton button) const = 0;

      /**
       * Interface for determining if a specific key was pressed since the last update.
       * 
       * TODO: Come up with a name that is not so lame.
       */
      virtual bool wasPressed(Key key) {
        return framePress.find(key) != framePress.end();
      }

      /**
       * Interface for determining if a specific mouse button was pressed since the last update.
       * 
       * TODO: Come up with a name that is not so lame.
       */
      virtual bool wasPressed(MouseButton button) {
        return frameMousePress.find(button) != frameMousePress.end();
      }

      /**
       * Interface for determining if a specific key was released since the last update.
       * 
       * TODO: Come up with a name that is not so lame.
       */
      virtual bool wasReleased(Key key) {
        return frameRelease.find(key) != frameRelease.end();
      }

      /**
       * Interface for determining if a specific mouse button was released since the last update.
       * 
       * TODO: Come up with a name that is not so lame.
       */
      virtual bool wasReleased(MouseButton button) {
        return frameMouseRelease.find(button) != frameMouseRelease.end();
      }

      /**
       * Interface for getting the current mouse postion at any point in time.
       */
      virtual math::Vector2 mousePosition() const = 0;

    protected:
      inline void pressed(Key key) {
        framePress.insert(key);
      }

      inline void released(Key key) {
        frameRelease.insert(key);
      }

      inline void pressed(MouseButton button) {
        frameMousePress.insert(button);
      }

      inline void released(MouseButton button) {
        frameMouseRelease.insert(button);
      }

      platform::Window* window;

      std::unordered_set<Key> framePress;

      std::unordered_set<Key> frameRelease;

      std::unordered_set<MouseButton> frameMousePress;

      std::unordered_set<MouseButton> frameMouseRelease;

      messaging::Messaging* messaging;
    };

  } // namespace input
} // namespace wage
