#include "input/input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include <iostream>
#include "platform/platform.h"
#include "core/system/context.h"
#include "core/logger.h"
#include "input/key_event.h"

namespace wage {
  
  Input* Input::Instance = nullptr;

  void Input::start(SystemContext* context) {
    Logger::info("Starting Input System");
    Input::Instance = this;
    messaging = context->get<Messaging>();
    Platform* platform = context->get<Platform>();
    window = platform->getWindow();
    glfwSetWindowUserPointer(window->as<GLFWwindow>(), this);
    glfwSetKeyCallback(window->as<GLFWwindow>(), [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
      } else if (action == GLFW_PRESS) {
        input->frameKeys.insert(key);
      } else if (action == GLFW_RELEASE) {
        input->frameKeys.erase(key);
      }
      if (input->messaging) {
        auto eventType = KeyEventType::press;
        switch (action) {
          case GLFW_RELEASE:
            eventType = KeyEventType::release;
            break;
          case GLFW_REPEAT:
            eventType = KeyEventType::repeat;
            break;
        }
        KeyEvent event(key, eventType);
        input->messaging->send(event);
      }
    });
    // Input::Instance->setMousePosition(Vector2(1024/2, 768/2));
    glfwSetInputMode(window->as<GLFWwindow>(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void Input::update(SystemContext* context) {
    if (glfwWindowShouldClose(window->as<GLFWwindow>())) {
      context->shouldStop();    
      return;
    }
  }

  bool Input::isPressed(int key) {
    return frameKeys.count(key) > 0;
  }

  Vector2 Input::mousePosition() {
    double x, y;
    glfwGetCursorPos(window->as<GLFWwindow>(), &x, &y);
    return Vector2(x, y); 
  }

  void  Input::setMousePosition(Vector2 position) {
    glfwSetCursorPos(window->as<GLFWwindow>(), position.x, position.y);
  }

}
