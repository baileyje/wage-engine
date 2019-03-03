#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <vector>

#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"
#include "component/camera/camera.h"

#include "math/transform.h"

#include "ecs/registry.h"

namespace wage {

  class RenderContext {

  public:

    RenderContext(Entity cameraEntity, Camera* camera, Vector2 screenSize,
      std::vector<Entity> dirLights, std::vector<Entity> pointLights, std::vector<Entity> spotlights) 
      :_cameraEntity(cameraEntity), _camera(camera), _screenSize(screenSize),
      _dirLights(dirLights), _pointLights(pointLights), _spotlights(spotlights) {
        auto camTransform = cameraEntity.get<Transform>();
        _screenProjection = camera->screenProjection(screenSize);
        _viewProjection = camera->viewProjection(camTransform.get());
        _cameraPosition = camTransform->position();
      }

    inline Camera* camera() {
      return _camera;
    }

    inline Entity cameraEntity() {
      return _cameraEntity;
    }

    inline Vector2 screenSize() {
      return _screenSize;
    }

    inline Matrix screenProjection() {
      return _screenProjection;
    }
    
    inline Vector cameraPosition() {
      return _cameraPosition;
    }
    
    inline Matrix viewProjection() {
      return _viewProjection;
    }

    inline std::vector<Entity> dirLights() {
      return _dirLights;
    }

    inline std::vector<Entity> pointLights() {
      return _pointLights;
    }

    inline std::vector<Entity> spotlights() {
      return _spotlights;
    }

  private:
  
    Entity _cameraEntity;

    Camera* _camera;

    Vector2 _screenSize;

    Matrix _screenProjection;
    
    Vector _cameraPosition;
    
    Matrix _viewProjection;

    std::vector<Entity> _dirLights;

    std::vector<Entity> _pointLights;

    std::vector<Entity> _spotlights;
  };

} 

#endif //RENDER_CONTEXT_H