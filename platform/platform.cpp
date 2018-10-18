#include "platform/platform.h"

void errorCallback(int error, const char* description) {
  printf("GLFW Error: %s\n", description);
}

Platform::~Platform() {
}

void Platform::init(Context* context)  {
  printf("Initializing Platform.\n");
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) {
    printf("GLFW Init Failed\n");
  }  
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, 1);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1024, 768, "Wage Bro!", NULL, NULL);
}

void Platform::deinit(Context* context) {
  printf("Deinitializing Platform.\n");
  glfwDestroyWindow(window);
  glfwTerminate();
}
