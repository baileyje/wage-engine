#pragma once

#include "component/color.h"

namespace wage {
  namespace component {

    class Light {

    public:
      Light(component::Color ambient, component::Color diffuse, component::Color specular)
          : _ambient(ambient), _diffuse(diffuse), _specular(specular) {
      }

      ~Light() {}

      component::Color ambient() const {
        return _ambient;
      }

      inline void ambient(component::Color ambient) {
        _ambient = ambient;
      }

      inline component::Color diffuse() const {
        return _diffuse;
      }

      inline void diffuse(component::Color diffuse) {
        _diffuse = diffuse;
      }

      inline component::Color specular() const {
        return _specular;
      }

      inline void specular(component::Color specular) {
        _specular = specular;
      }

    protected:
      component::Color _ambient;

      component::Color _diffuse;

      component::Color _specular;
    };
  }
}