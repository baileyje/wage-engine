#pragma once

#include "component/lighting/light.h"

#define PointLightComponent 21

namespace wage {
  namespace component {

    class PointLight : public Light {

    public:
      PointLight()
          : PointLight(Color(0.05f, 0.05f, 0.05f, 1), Color(0.8f, 0.8f, 0.8f, 1), Color::White, 1.0f, 0.09, 0.032) {
      }

      PointLight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic)
          : Light(ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic) {
      }

      ~PointLight() {}

      inline void constant(float constant) {
        _constant = constant;
      }

      inline float constant() const {
        return _constant;
      }

      inline void linear(float linear) {
        _linear = linear;
      }

      inline float linear() const {
        return _linear;
      }

      inline void quadratic(float quadratic) {
        _quadratic = quadratic;
      }

      inline float quadratic() const {
        return _quadratic;
      }

    private:
      float _constant;

      float _linear;

      float _quadratic;
    };

  }
}