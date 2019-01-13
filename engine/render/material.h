#ifndef ENTITY_COMPONENT_MATERIAL_H
#define ENTITY_COMPONENT_MATERIAL_H

#include "entity/component.h"
#include "math/color.h"
#include "render/texture.h"

namespace wage {
  // TODO:  De-jank this
  class Material : public Component {

  public:

    Material() : Material(Color(1, 0, 1, 1)) {
    }

    Material(Color color) : Component("Material"), color_(color) {    
    }

    Material(Texture* texture) : Component("Material"), color_(Color(1, 0, 1, 1)), texture_(texture) {    
    }

    ~Material() {}

    inline void setColor(Color color) {
      color_ = color;
    }

    inline Color color()  {
      return color_;
    }

    inline Texture*  texture()  {
      return texture_;
    }

  private:
  
    Color color_;

    Texture* texture_;

  };

}

#endif //ENTITY_COMPONENT_MATERIAL_H