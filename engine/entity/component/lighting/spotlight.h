#ifndef COMPONENT_SPOTLIGHT_H
#define COMPONENT_SPOTLIGHT_H

#include "engine/entity/component/lighting/light.h"

namespace wage {

  class Spotlight : public Light {

  public:

    Spotlight() 
      : Spotlight(Color(0.0f, 0.0f, 0.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), 1.0f, 0.99, 0.032, 12.5, 15.0) {    
    }

    Spotlight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff) 
      : Light("Spotlight", ambient, diffuse, specular), constant(constant), linear(linear), quadratic(quadratic), cutOff(cutOff), outerCutOff(outerCutOff)  {    
    }

    ~Spotlight() {}
    
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

    inline void setCutOff(float cutOff) {
      this->cutOff = cutOff;
    }

    inline float getCutOff() {
      return cutOff;
    }

    inline void setOuterCutOff(float cutOff) {
      this->outerCutOff = cutOff;
    }

    inline float getOuterCutOff() {
      return outerCutOff;
    }

  private:

      float constant;
      
      float linear;
      
      float quadratic;

      float cutOff;

      float outerCutOff;
  };

}

#endif //COMPONENT_SPOTLIGHT_H

 