#pragma once

#include <vector>

#include "render/components/lighting/directional_light.h"
#include "render/components/lighting/point_light.h"
#include "render/components/lighting/spotlight.h"
#include "render/components/camera/camera.h"

#include "math/transform.h"
#include "math/frustum.h"

#include "ecs/registry.h"

namespace wage {
  namespace render {

    /**
     * Contextual data used to render a frame to the screen. This information is a snap shot of data extracted from the game update state and should not be referenced
     * outside the renderer during the frame rendering.
     */
    class RenderContext {

    public:
      RenderContext(ecs::Entity cameraEntity, Camera* camera, math::Vector2 screenSize, std::vector<ecs::Entity> dirLights, std::vector<ecs::Entity> pointLights, std::vector<ecs::Entity> spotlights)
          : _screenSize(screenSize), _dirLights(dirLights), _pointLights(pointLights), _spotlights(spotlights) {
        auto camTransform = cameraEntity.get<math::Transform>(TransformComponent);
        _screenProjection = camera->screenProjection(screenSize);
        _viewProjection = camera->viewProjection(camTransform);
        _cameraPosition = camTransform->position();
        _cameraFrustum = camera->frustum(screenSize, camTransform);
      }

      /**
       * The current screensize to render to.
       */
      inline math::Vector2 screenSize() {
        return _screenSize;
      }

      /**
       * The current screen projection with respect to the camera.
       */
      inline math::Matrix screenProjection() {
        return _screenProjection;
      }

      /**
       * The position of the camera within the world.
       */
      inline math::Vector cameraPosition() {
        return _cameraPosition;
      }

      /**
       * The current cameral look projection.
       */
      inline math::Matrix viewProjection() {
        return _viewProjection;
      }

      /**
       * The relevent directional lights for the rendering.
       */
      inline std::vector<ecs::Entity> dirLights() {
        return _dirLights;
      }

      /**
       * The relevent point lights for the rendering.
       */
      inline std::vector<ecs::Entity> pointLights() {
        return _pointLights;
      }

      /**
       * The relevent spot lights for the rendering.
       */
      inline std::vector<ecs::Entity> spotlights() {
        return _spotlights;
      }

      /**
       * The cameras frustum used for culling the scene.
       */
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