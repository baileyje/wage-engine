#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "entity/component.h"
#include "math/color.h"

namespace wage {

  class Light : public Component {

  public:

    Light(std::string name, Color ambient, Color diffuse, Color specular) 
      : Component(name), _ambient(ambient), _diffuse(diffuse), _specular(specular) {    
    }

    ~Light() {}

    Color* ambient() {
      return &_ambient;
    }

    inline void ambient(Color ambient) {
      _ambient = ambient;
    }

    inline Color* diffuse() {
      return &_diffuse;
    }

    inline void diffuse(Color diffuse) {
      _diffuse = diffuse;
    }

    inline Color* specular() {
      return &_specular;
    }

    inline void specular(Color specular) {
      _specular = specular;
    }

  protected:

    Color _ambient;

    Color _diffuse;

    Color _specular;


  };

}

#endif //COMPONENT_LIGHT_H