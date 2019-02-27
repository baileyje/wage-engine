#ifndef INPUT_H
#define INPUT_H

#include <unordered_set>

#include "core/service.h"
#include "messaging/messaging.h"
#include "platform/window.h"
#include "math/vector.h"

#include "input/key.h"

namespace wage {

  class Input : public Service {

  public:
    
    static Input* Instance;

    Input() : Service("input"), window(nullptr), messaging(nullptr) { }

    ~Input() {}

    void start();
    
    bool isPressed(Key key);

    Vector2 mousePosition();

  private: 
    
    Window* window;
    
    std::unordered_set<int> frameKeys;

    Messaging* messaging;

  };

}

#endif //INPU T_H