#pragma once

#include "ui/frame.h"

namespace wage { namespace ui {

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

} }
