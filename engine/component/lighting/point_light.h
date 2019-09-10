#pragma once

#include "component/lighting/light.h"

namespace wage { namespace component {

  class PointLight : public Light {

  public:
    PointLight()
        : PointLight(math::Color(0.05f, 0.05f, 0.05f, 1), math::Color(0.8f, 0.8f, 0.8f, 1), math::Color::White, 1.0f, 0.09, 0.032) {
    }

    PointLight(math::Color ambient, math::Color diffuse, math::Color specular, float constant, float linear, float quadratic)
        : Light(ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic) {
    }

    ~PointLight() {}

    inline void constant(float constant) {
      _constant = constant;
    }

    inline float constant() {
      return _constant;
    }

    inline void linear(float linear) {
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

  private:
    float _constant;

    float _linear;

    float _quadratic;
  };

} }