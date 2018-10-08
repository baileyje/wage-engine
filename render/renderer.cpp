#include "render/renderer.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif

void errorCallback(int error, const char* description) {
  printf("Error: %s\n", description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Renderer::init(Context* context)  {
  printf("Initializing Renderer.\n");
  glfwSetErrorCallback(errorCallback);
  if (!glfwInit()) {
    printf("GLFW Init Failed\n");
  }  
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  // glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
  glfwSetKeyCallback(window, keyCallback);  
}

void Renderer::start(Context* context) {
  printf("Starting Renderer.\n");
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::update(Context* context) {
  // printf("Updating Renderer.\n");
  if (glfwWindowShouldClose(window)) {
    printf("Close!\n");
    context->shouldStop();
    return;
  }
  float ratio;
  int width, height;
  // mat4x4 m, p, mvp;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void Renderer::stop(Context* context) {
  printf("Stopping Renderer.\n");
}

void Renderer::deinit(Context* context) {
  printf("Deinitializing Renderer.\n");
  glfwDestroyWindow(window);
  glfwTerminate();
}

