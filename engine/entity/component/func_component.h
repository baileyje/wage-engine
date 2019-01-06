#ifndef COMPONENT_FUNC_H
#define COMPONENT_FUNC_H

#include <sstream>
#include <functional>

#include "engine/entity/component/dynamic.h"

namespace wage {

  class FunctionComponent : public DynamicComponent {

  public:

    FunctionComponent(std::function<void(ComponentContext* context)> compCallback) : DynamicComponent(FunctionComponent::nextName()), compCallback(compCallback) {
    }

    void update(ComponentContext* context) {
      compCallback(context);
    }

  private:

    ComponentCallback compCallback;

    static std::string nextName() {
      static int idx = 0;
      std::stringstream stream;
      stream << "FunctionComponent-0" << idx++;
      return stream.str();
    }

  };

}

#endif //COMPONENT_FUNC_H
