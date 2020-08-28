#pragma once

#include "component/lighting/light.h"

#define SpotlightComponent 20

namespace wage {
  namespace component {

    class Spotlight : public Light {

    public:
      Spotlight()
          : Spotlight(Color(0.0f, 0.0f, 0.0f, 1), Color::White, Color::White, 1.0f, 0.99, 0.032, 12.5, 15.0) {
      }

      Spotlight(Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, float cutOff, float outerCutOff)
          : Light(ambient, diffuse, specular), _constant(constant), _linear(linear), _quadratic(quadratic), _cutOff(cutOff), _outerCutOff(outerCutOff) {
      }

      ~Spotlight() {}

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

      inline void cutOff(float cutOff) {
        _cutOff = cutOff;
      }

      inline float cutOff() const {
        return _cutOff;
      }

      inline void outerCutOff(float cutOff) {
        _outerCutOff = cutOff;
      }

      inline float outerCutOff() const {
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
}
