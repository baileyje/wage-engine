#ifndef COMPONENT_FUNC_H
#define COMPONENT_FUNC_H

#include <sstream>
#include <functional>

#include "entity/component/context.h"
#include "entity/component.h"


typedef void (*ComponentCallback)(ComponentContext*);


class FunctionComponent : public Component {

public:

  FunctionComponent(ComponentCallback compCallback) : Component(FunctionComponent::nextName()), compCallback(compCallback) {
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

#endif //COMPONENT_FUNC_H



