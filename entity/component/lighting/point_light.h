#ifndef COMPONENT_POINT_LIGHT_H
#define COMPONENT_POINT_LIGHT_H

#include "entity/component/lighting/light.h"

namespace wage {

  class PointLight : public Light {

  public:

    PointLight() 
      : PointLight(Color(0.05f, 0.05f, 0.05f, 1), Color(0.8f, 0.8f, 0.8f, 1), Color(1.0f, 1.0f, 1.0f, 1), 1.0f, 0.09, 0.032) {    
    }

    PointLight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic) 
      : Light("PointLight", ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic)  {    
    }

    ~PointLight() {}
    
    inline void setConstant(float constant) {
      this->constant = constant;
    }

    inline float getConstant() {
      return constant;
    }

    inline void setLinear(float linear) {
      this->linear = linear;
    }

    inline float getLinear() {
      return linear;
    }

    inline void setQuadratic(float quadratic) {
      this->quadratic = quadratic;
    }

    inline float getQuadratic() {
      return quadratic;
    }

  private:

      float constant;
      
      float linear;
      
      float quadratic;

  };

}

#endif //COMPONENT_POINT_LIGHT_H