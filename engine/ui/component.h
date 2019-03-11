#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "ui/frame.h"

namespace wage {

  class UiComponent {

  public:
    UiComponent() {
    }

    UiComponent(UiFrame frame) : _frame(frame) {}

    virtual ~UiComponent() {}

    inline UiFrame frame() const {
      return _frame;
    }

    inline void frame(UiFrame frame) {
      _frame = frame;
    }

  private:
    UiFrame _frame;
  };
}

#endif // UI_COMPONENT_H