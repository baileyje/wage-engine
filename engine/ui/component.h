#ifndef COMPONENT_UI_COMPONENT_H
#define COMPONENT_UI_COMPONENT_H

#include "entity/component/dynamic.h"
#include "ui/frame.h"

namespace wage {

  class UiComponent : public DynamicComponent {

  public:

    UiComponent(std::string name) : DynamicComponent(name) {
    }

    UiComponent(std::string name, Frame frame) : DynamicComponent(name), _frame(frame) {}

    virtual ~UiComponent() {}

    inline Frame frame() {
      return _frame;
    }

  private:

    Frame _frame;

  };

}

#endif //COMPONENT_UI_COMPONENT_H