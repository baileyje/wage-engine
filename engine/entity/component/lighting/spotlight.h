#ifndef COMPONENT_SPOTLIGHT_H
#define COMPONENT_SPOTLIGHT_H

#include "entity/component/lighting/light.h"

namespace wage {

  class Spotlight : public Light {

  public:

    Spotlight() 
      : Spotlight(Color(0.0f, 0.0f, 0.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), 1.0f, 0.99, 0.032, 12.5, 15.0) {    
    }

    Spotlight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff) 
      : Light("Spotlight", ambient, diffuse, specular), constant_(constant), linear_(linear), quadratic_(quadratic), cutOff_(cutOff), outerCutOff_(outerCutOff)  {    
    }

    ~Spotlight() {}
    
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

    inline void setCutOff(float cutOff) {
      cutOff_ = cutOff;
    }

    inline float cutOff() {
      return cutOff_;
    }

    inline void setOuterCutOff(float cutOff) {
      outerCutOff_ = cutOff;
    }

    inline float outerCutOff() {
      return outerCutOff_;
    }

  private:

      float constant_;
      
      float linear_;
      
      float quadratic_;

      float cutOff_;

      float outerCutOff_;
  };

}

#endif //COMPONENT_SPOTLIGHT_H

 