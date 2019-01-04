#ifndef INPUT_H
#define INPUT_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <unordered_set>

#include "core/system.h"
#include "platform/window.h"
#include "math/vector.h"

namespace wage {

  class Input : public System {

  public:
    
    static Input* Instance;

    Input() : System("input") { }

    ~Input() {}

    LIFECYCLE_FUNC(start)
    
    LIFECYCLE_FUNC(update)

    bool isPressed(int key);

    Vector2 mousePosition();
    
    void setMousePosition(Vector2 position);

  private: 
    
    Window* window;
    
    std::unordered_set<int> frameKeys;

  };

}

#endif //INPU T_H