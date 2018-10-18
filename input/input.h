#ifndef INPUT_H
#define INPUT_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <unordered_set>

#include "core/system.h"

class Input : public System {

public:
  
  Input() : System("input") { }

  ~Input() {}

  void start(Context* context);

  void update(Context* context);

  static bool isPressed(int key);

private: 
  
  GLFWwindow* window;
  
};

#endif //INPUT_H