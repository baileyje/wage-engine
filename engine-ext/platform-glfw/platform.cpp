#include "platform-glfw/platform.h"

#include "core/core.h"
#include "core/logger.h"
#include "memory/allocator.h"

namespace wage {
  namespace platform {

    void errorCallback(int error, const char* description) {
      core::Logger::error("GLFW Error: %s", description);
    }

    void GlfwPlatform::start() {
      glfwSetErrorCallback(errorCallback);
      if (!glfwInit()) {
        core::Logger::error("GLFW Init Failed.");
      }
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      GLFWwindow* glfWindow = glfwCreateWindow(1024, 768, "Wage is the Rage!", NULL, NULL);
      int screenWidth = 0;
      int screenHeight = 0;
      glfwGetWindowSize(glfWindow, &screenWidth, &screenHeight);
      int frameBufferWidth = 0;
      int frameBufferHeight = 0;
      glfwGetFramebufferSize(glfWindow, &frameBufferWidth, &frameBufferHeight);
      float scale = (float)frameBufferWidth / (float)screenWidth;
      _window = new Window(glfWindow, frameBufferWidth, frameBufferHeight, scale);

      core::Core::Instance->onInput([&](const core::Frame& frame) {
        flushEvents();
      });
    }

    void GlfwPlatform::stop() {
      glfwDestroyWindow(_window->as<GLFWwindow>());
      glfwTerminate();
      // TODO: Cleanup window
      // delete window;
    }

  }
}
