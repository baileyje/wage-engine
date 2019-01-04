#include "input/input.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "platform/platform.h"
#include "core/system/context.h"
#include "core/logger.h"

namespace wage {
  
  Input* Input::Instance = nullptr;

  void Input::start(SystemContext* context) {
    Logger::info("Starting Input System");
    Input::Instance = this;
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
