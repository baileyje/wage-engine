#ifndef ECS_FUNC_SYSTEM
#define ECS_FUNC_SYSTEM

#include <functional>
#include <string>
#include <iostream>
#include <typeindex>

#include "ecs/system.h"

namespace wage {

  
  class FuncSystem : public System {
  
  public:
  
    enum class Event { Update, FixedUpdate };

    FuncSystem() : System("FuncSystem") {}

    void update(SystemContext* context) override {
      // std::cout << "Func: " << std::typeid(this).name() << std::endl;
    }
  };

}

#endif // ECS_FUNC_SYSTEM