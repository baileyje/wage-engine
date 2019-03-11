#include "platform-glfw/platform.h"

#include "core/logger.h"
#include "memory/allocator.h"

namespace wage {

  void errorCallback(int error, const char* description) {
    Logger::error("GLFW Error: %s", description);
  }

  void GlfwPlatform::start() {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
      Logger::error("GLFW Init Failed.");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfWindow = glfwCreateWindow(1024, 768, "Wage is the Rage!", NULL, NULL);
    int screenWidth = 0;
    int screenHeight = 0;
    glfwGetFramebufferSize(glfWindow, &screenWidth, &screenHeight);
    _window = make<Window>(glfWindow, screenWidth, screenHeight);
  }

  void GlfwPlatform::stop() {
    glfwDestroyWindow(_window->as<GLFWwindow>());
    glfwTerminate();
    // TODO: Cleanup window
    // delete window;
  }
}
