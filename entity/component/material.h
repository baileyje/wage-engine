#ifndef ENTITY_COMPONENT_MATERIAL_H
#define ENTITY_COMPONENT_MATERIAL_H

#include "entity/component.h"
#include "entity/color.h"
#include "entity/component/texture.h"

// TODO:  De-jank this
class Material : public Component {

public:

  Material() : Material(Color(1, 0, 1, 1)) {
  }

  Material(Color color) : Component("Material"), color(color) {    
  }

  Material(Texture* texture) : Component("Material"), texture(texture), color(Color(1, 0, 1, 1)) {    
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

#endif //ENTITY_COMPONENT_MATERIAL_H