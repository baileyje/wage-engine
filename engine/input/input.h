#ifndef INPUT_H
#define INPUT_H

#include <unordered_set>

#include "engine/core/system.h"
#include "engine/messaging/messaging.h"
#include "engine/platform/window.h"
#include "engine/math/vector.h"

namespace wage {

  class Input : public System {

  public:
    
    static Input* Instance;

    Input() : System("input"), window(nullptr), messaging(nullptr) { }

    ~Input() {}

    LIFECYCLE_FUNC(start)
    
    LIFECYCLE_FUNC(update)

    bool isPressed(int key);

    Vector2 mousePosition();

  private: 
    
    Window* window;
    
    std::unordered_set<int> frameKeys;

    Messaging* messaging;

  };

}

#endif //INPU T_H