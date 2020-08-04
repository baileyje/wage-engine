#pragma once

#include <unordered_set>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"

#include "input/key.h"
#include "input/input.h"

namespace wage { namespace input {

  class GlfwInput : public Input {

  public:
    GlfwInput() : Input() {}

    ~GlfwInput() {}

    void start() override;

    bool isPressed(Key key) const override;

    math::Vector2 mousePosition() const override;
  };
} }