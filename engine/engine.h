#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "core/system.h"
#include "entity/entity.h"

namespace wage {

  class Engine : public System {

  public:
    
    Engine() : System("Engine") { }

    ~Engine() {}

    // LIFECYCLE_FUNC(start)
    // LIFECYCLE_FUNC(update)
    // LIFECYCLE_FUNC(fixedUpdate)
    // LIFECYCLE_FUNC(stop)

  };

}

#endif //ENGINE_H