#ifndef COMPONENT_SPOTLIGHT_H
#define COMPONENT_SPOTLIGHT_H

#include "component/lighting/light.h"

namespace wage {

  class Spotlight : public Light {

  public:

    Spotlight() 
      : Spotlight(Color(0.0f, 0.0f, 0.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), Color(1.0f, 1.0f, 1.0f, 1), 1.0f, 0.99, 0.032, 12.5, 15.0) {    
    }

    Spotlight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff) 
      : Light(ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic), _cutOff(cutOff), _outerCutOff(outerCutOff)  {    
    }

    ~Spotlight() {}
    
    inline void constant(float constant) {
      _constant = constant;
    }

    inline float constant() {
      return _constant;
    }

    inline void lLinear(float linear) {
      _linear = linear;
    }

    inline float linear() {
      return _linear;
    }

    inline void quadratic(float quadratic) {
      _quadratic = quadratic;
    }

    inline float quadratic() {
      return _quadratic;
    }

    inline void cutOff(float cutOff) {
      _cutOff = cutOff;
    }

    inline float cutOff() {
      return _cutOff;
    }

    inline void outerCutOff(float cutOff) {
      _outerCutOff = cutOff;
    }

    inline float outerCutOff() {
      return _outerCutOff;
    }

  private:

      float _constant;
      
      float _linear;
      
      float _quadratic;

      float _cutOff;

      float _outerCutOff;
  
  };
}

#endif //COMPONENT_SPOTLIGHT_H

 