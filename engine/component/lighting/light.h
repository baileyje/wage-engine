#pragma once

#include "math/color.h"

namespace wage { namespace component {

  class Light {

  public:
    Light(math::Color ambient, math::Color diffuse, math::Color specular)
        : _ambient(ambient), _diffuse(diffuse), _specular(specular) {
    }

    ~Light() {}

    math::Color* ambient() {
      return &_ambient;
    }

    inline void ambient(math::Color ambient) {
      _ambient = ambient;
    }

    inline math::Color* diffuse() {
      return &_diffuse;
    }

    inline void diffuse(math::Color diffuse) {
      _diffuse = diffuse;
    }

    inline math::Color* specular() {
      return &_specular;
    }

    inline void specular(math::Color specular) {
      _specular = specular;
    }

  protected:
    math::Color _ambient;

    math::Color _diffuse;

    math::Color _specular;
  };
} }