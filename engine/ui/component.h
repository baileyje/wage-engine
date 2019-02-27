#ifndef COMPONENT_UI_COMPONENT_H
#define COMPONENT_UI_COMPONENT_H

#include "entity/component.h"
#include "ui/frame.h"

namespace wage {

  class UiComponent : public Component {

  public:

    UiComponent(std::string name) : Component(name) {
    }

    UiComponent(std::string name, LayoutFrame frame) : Component(name), _frame(frame) {}

    virtual ~UiComponent() {}

    inline LayoutFrame& frame() {
      return _frame;
    }

  private:

    LayoutFrame _frame;

  };

}

#endif //COMPONENT_UI_COMPONENT_H