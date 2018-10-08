#ifndef RENDERER_H
#define RENDERER_H

// #include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/system.h"

class Renderer : public System {

public:

  Renderer() : System("renderer") { }

  void init(Context* context);

  void start(Context* context);

  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

private:

  GLFWwindow* window;
  
};

#endif //RENDERER_H