#ifndef ENTITY_COMPONENT_MATERIAL_H
#define ENTITY_COMPONENT_MATERIAL_H

#include "engine/entity/component.h"
#include "engine/math/color.h"
#include "engine/render/texture.h"

namespace wage {
  // TODO:  De-jank this
  class Material : public Component {

  public:

    Material() : Material(Color(1, 0, 1, 1)) {
    }

    Material(Color color) : Component("Material"), color(color) {    
    }

    Material(Texture* texture) : Component("Material"), color(Color(1, 0, 1, 1)), texture(texture) {    
    }

    ~Material() {}

    inline void setColor(Color color) {
      this->color = color;
    }

    inline Color getColor()  {
      return color;
    }

    inline Texture*  getTexture()  {
      return texture;
    }

  private:
  
    Color color;

    Texture* texture;

  };

}

#endif //ENTITY_COMPONENT_MATERIAL_H