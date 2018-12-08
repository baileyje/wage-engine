#ifndef COMPONENT_DIR_LIGHT_H
#define COMPONENT_DIR_LIGHT_H

#include "entity/component/lighting/light.h"

namespace wage {

  class DirectionalLight : public Light {

  public:

    DirectionalLight() : DirectionalLight(Color(0.05f, 0.05f, 0.05f, 1), Color(0.4f, 0.4f, 0.4f, 1), Color(0.5f, 0.5f, 0.5f, 1)) {    
    }

    DirectionalLight(Color ambient, Color diffuse, Color specular) 
      : Light("DirectionalLight", ambient, diffuse, specular) {    
    }

    ~DirectionalLight() {}
  };

}

#endif //COMPONENT_DIR_LIGHT_H