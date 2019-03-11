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
    Input() : Service("Input"), window(nullptr), messaging(nullptr) {}

    ~Input() {}

    void start();

    virtual bool isPressed(Key key) = 0;

    virtual Vector2 mousePosition() = 0;

  protected:
    Window* window;

    std::unordered_set<int> frameKeys;

    Messaging* messaging;
  };
}

#endif //INPU T_H