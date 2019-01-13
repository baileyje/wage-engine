#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "entity/component.h"
#include "math/color.h"

namespace wage {

  class Light : public Component {

  public:

    Light(std::string name, Color ambient, Color diffuse, Color specular) 
      : Component(name), ambient_(ambient), diffuse_(diffuse), specular_(specular) {    
    }

    ~Light() {}

    Color* ambient() {
      return &ambient_;
    }

    inline void setAmbient(Color ambient) {
      ambient_ = ambient;
    }

    inline Color* diffuse() {
      return &diffuse_;
    }

    inline void setDiffuse(Color diffuse) {
      diffuse_ = diffuse;
    }

    inline Color* specular() {
      return &specular_;
    }

    inline void setSpecular(Color specular) {
      specular_ = specular;
    }

  protected:

    Color ambient_;

    Color diffuse_;

    Color specular_;


  };

}

#endif //COMPONENT_LIGHT_H