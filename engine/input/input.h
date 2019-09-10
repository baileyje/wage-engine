#pragma once

#include <unordered_set>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"

#include "input/key.h"

namespace wage { namespace input {

  class Input : public core::Service {

  public:
    Input() : Service("Input"), window(nullptr), messaging(nullptr) {}

    ~Input() {}

    void start();

    virtual bool isPressed(Key key) = 0;

    virtual math::Vector2 mousePosition() = 0;

  protected:
    platform::Window* window;

    std::unordered_set<int> frameKeys;

    messaging::Messaging* messaging;
  };

} }