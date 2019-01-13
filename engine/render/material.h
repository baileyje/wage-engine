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

    Material(Color color) : Component("Material"), _color(color) {    
    }

    Material(Texture* texture) : Component("Material"), _color(Color(1, 0, 1, 1)), _texture(texture) {    
    }

    ~Material() {}

    inline void color(Color color) {
      _color = color;
    }

    inline Color color()  {
      return _color;
    }

    inline Texture*  texture()  {
      return _texture;
    }

  private:
  
    Color _color;

    Texture* _texture;

  };

}

#endif //ENTITY_COMPONENT_MATERIAL_H