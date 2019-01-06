#ifndef COMPONENT_UI_COMPONENT_H
#define COMPONENT_UI_COMPONENT_H

#include "engine/entity/component/dynamic.h"
#include "engine/ui/frame.h"

namespace wage {

  class UiComponent : public DynamicComponent {

  public:

    UiComponent(std::string name) : DynamicComponent(name) {
    }

    UiComponent(std::string name, Frame frame) : DynamicComponent(name), frame_(frame) {}

    virtual ~UiComponent() {}

    inline Frame frame() {
      return frame_;
    }

  private:

    Frame frame_;

  };

}

#endif //COMPONENT_UI_COMPONENT_H