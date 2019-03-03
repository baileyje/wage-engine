#ifndef COMPONENT_UI_COMPONENT_H
#define COMPONENT_UI_COMPONENT_H

#include "ui/frame.h"

namespace wage {

  class UiComponent {

  public:

    UiComponent() {
    }

    UiComponent(LayoutFrame frame) : _frame(frame) {}

    virtual ~UiComponent() {}

    inline LayoutFrame& frame() {
      return _frame;
    }

  private:

    LayoutFrame _frame;

  };

}

#endif //COMPONENT_UI_COMPONENT_H