#ifndef COMPONENT_FUNC_H
#define COMPONENT_FUNC_H

#include <sstream>
#include <functional>

#include "ecs/system.h"

namespace wage {

  class SystemContext;
  
  typedef std::function<void(SystemContext* context)> SystemCallback;

  class FunctionSystem : public System {

  public:

    FunctionSystem(std::function<void(SystemContext* context)> compCallback) : System(FunctionSystem::nextName()), compCallback(compCallback) {
    }

    void update(SystemContext* context) {
      compCallback(context);
    }

  private:

    SystemCallback compCallback;

    static std::string nextName() {
      static int idx = 0;
      std::stringstream stream;
      stream << "FunctionSystem-0" << idx++;
      return stream.str();
    }

  };

}

#endif //COMPONENT_FUNC_H
