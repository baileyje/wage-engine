#pragma once

#include <vector>

#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"
#include "component/camera/camera.h"

#include "math/transform.h"
#include "math/frustum.h"

#include "ecs/registry.h"

namespace wage {
  namespace render {

    class RenderContext {

    public:
      RenderContext(ecs::Entity cameraEntity, component::Camera* camera, math::Vector2 screenSize,
                    std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights)
          : _screenSize(screenSize),
            _dirLights(dirLights), _pointLights(pointLights), _spotlights(spotlights) {
        auto camTransform = cameraEntity.get<math::Transform>(TransformComponent);
        _screenProjection = camera->screenProjection(screenSize);
        _viewProjection = camera->viewProjection(camTransform);
        _cameraPosition = camTransform->position();
        _cameraFrustum = camera->frustum(screenSize, camTransform);
      }

      inline math::Vector2 screenSize() {
        return _screenSize;
      }

      inline math::Matrix screenProjection() {
        return _screenProjection;
      }

      inline math::Vector cameraPosition() {
        return _cameraPosition;
      }

      inline math::Matrix viewProjection() {
        return _viewProjection;
      }

      inline std::vector<ecs::Entity> dirLights() {
        return _dirLights;
      }

      inline std::vector<ecs::Entity> pointLights() {
        return _pointLights;
      }

      inline std::vector<ecs::Entity> spotlights() {
        return _spotlights;
      }

      inline math::Frustum cameraFrustum() {
        return _cameraFrustum;
      }

    private:
      math::Vector2 _screenSize;

      math::Matrix _screenProjection;

      math::Vector3 _cameraPosition;

      math::Frustum _cameraFrustum;

      math::Matrix _viewProjection;

      std::vector<ecs::Entity> _dirLights;

      std::vector<ecs::Entity> _pointLights;

      std::vector<ecs::Entity> _spotlights;
    };

  }
}