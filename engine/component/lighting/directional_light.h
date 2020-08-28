#pragma once

#include "component/lighting/light.h"

#define DirectionalLightComponent 22

namespace wage {
  namespace component {

    class DirectionalLight : public Light {

    public:
      DirectionalLight() : DirectionalLight(Color(0.05f, 0.05f, 0.05f, 1), Color(0.4f, 0.4f, 0.4f, 1), Color(0.5f, 0.5f, 0.5f, 1)) {
      }

      DirectionalLight(Color ambient, Color diffuse, Color specular)
          : Light(ambient, diffuse, specular) {
      }

      ~DirectionalLight() {}
    };

  }
}