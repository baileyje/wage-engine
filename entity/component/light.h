#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "entity/component.h"
#include "entity/color.h"

class Light : public Component {

public:

  Light(std::string name, Color ambient, Color diffuse, Color specular) 
    : Component(name), ambient(ambient), diffuse(diffuse), specular(specular) {    
  }

  ~Light() {}

   Color* getAmbient() {
    return &ambient;
  }

  inline void setAmbient(Color ambient) {
    this->ambient = ambient;
  }

  inline Color* getDiffuse() {
    return &diffuse;
  }

  inline void setDiffuse(Color diffuse) {
    this->diffuse = diffuse;
  }

  inline Color* getSpecular() {
    return &specular;
  }

  inline void setSpecular(Color specular) {
    this->specular = specular;
  }

protected:

  Color ambient;

  Color diffuse;

  Color specular;


};

#endif //COMPONENT_LIGHT_H