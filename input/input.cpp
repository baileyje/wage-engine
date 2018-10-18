#include "input/input.h"

#include <iostream>
#include <GLFW/glfw3.h>
#include "platform/platform.h"

static std::unordered_set<int> frameKeys;

void Input::start(Context* context) {
  printf("Starting Input System\n");
  Platform* platform = static_cast<Platform*>(context->get("platform"));
  window = platform->getWindow();
  glfwSetWindowUserPointer(window, this);
  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      return;
    } else if (action == GLFW_PRESS) {
      frameKeys.insert(key);
    } else if (action == GLFW_RELEASE) {
      frameKeys.erase(key);
    }
  });
}

void Input::update(Context* context) {
  if (glfwWindowShouldClose(window)) {
    printf("Close!\n");
    context->shouldStop();    
    return;
  }
}

bool Input::isPressed(int key) {
  return frameKeys.count(key) > 0;
}

