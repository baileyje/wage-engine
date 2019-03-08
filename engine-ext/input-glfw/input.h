#ifndef GLFW_INPUT_H
#define GLFW_INPUT_H

#include <unordered_set>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"

#include "input/key.h"
#include "input/input.h"

namespace wage {

  class GlfwInput : public Input {

  public:
    
    GlfwInput() : Input() { }

    ~GlfwInput() {}

    void start() override;
    
    bool isPressed(Key key) override;

    Vector2 mousePosition() override;

  };

}

#endif // GLFW_INPUT_H