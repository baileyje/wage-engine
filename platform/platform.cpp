#include "platform/platform.h"

#include "core/logger.h"

namespace wage {

  void errorCallback(int error, const char* description) {
    Logger::error("GLFW Error: %s", description);
  }

  Platform::~Platform() {
  }

  void Platform::init(SystemContext* context)  {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
      Logger::error("GLFW Init Failed.");
    }  
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Wage is the Rage!", NULL, NULL);
  }

  void Platform::deinit(SystemContext* context) {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

}
