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
      : Light("PointLight", ambient, diffuse, specular), constant_(constant), linear_(linear), quadratic_(quadratic)  {    
    }

    ~PointLight() {}
    
    inline void setConstant(float constant) {
      constant_ = constant;
    }

    inline float constant() {
      return constant_;
    }

    inline void setLinear(float linear) {
      linear_ = linear;
    }

    inline float linear() {
      return linear_;
    }

    inline void setQuadratic(float quadratic) {
      quadratic_ = quadratic;
    }

    inline float quadratic() {
      return quadratic_;
    }

  private:

      float constant_;
      
      float linear_;
      
      float quadratic_;

  };

}

#endif //COMPONENT_POINT_LIGHT_H