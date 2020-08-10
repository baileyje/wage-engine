#pragma once

#include "component/lighting/light.h"

#define DirectionalLightComponent 22

namespace wage { namespace component {

  class DirectionalLight : public Light {

  public:
    DirectionalLight() : DirectionalLight(math::Color(0.05f, 0.05f, 0.05f, 1), math::Color(0.4f, 0.4f, 0.4f, 1), math::Color(0.5f, 0.5f, 0.5f, 1)) {
    }

    DirectionalLight(math::Color ambient, math::Color diffuse, math::Color specular)
        : Light(ambient, diffuse, specular) {
    }

    ~DirectionalLight() {}
  };

} }