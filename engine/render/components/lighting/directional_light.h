#pragma once

#include "render/components/lighting/light.h"

#define DirectionalLightComponent 22

namespace wage {
  namespace render {

    class DirectionalLight : public Light {

    public:
      DirectionalLight() : DirectionalLight(component::Color(0.05f, 0.05f, 0.05f, 1), component::Color(0.4f, 0.4f, 0.4f, 1), component::Color(0.5f, 0.5f, 0.5f, 1)) {
      }

      DirectionalLight(component::Color ambient, component::Color diffuse, component::Color specular)
          : Light(ambient, diffuse, specular) {
      }

      ~DirectionalLight() {}
    };

  }
}