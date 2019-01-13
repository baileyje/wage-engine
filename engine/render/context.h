#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <vector>

#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"
#include "entity/component/camera/camera.h"

namespace wage {

  class RenderContext {

  public:

    RenderContext(ComponentReference<Camera> camera, Vector2 screenSize,
      std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights, std::vector<Spotlight*> spotlights) 
      : _camera(camera), _screenSize(screenSize),
      _dirLights(dirLights), _pointLights(pointLights), _spotlights(spotlights) {
          _screenProjection = camera->screenProjection(screenSize);
          _viewProjection = camera->viewProjection();
          _cameraPosition = camera->transform()->position();
      }

    inline ComponentReference<Camera> camera() {
      return _camera;
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

    inline std::vector<DirectionalLight*> dirLights() {
      return _dirLights;
    }

    inline std::vector<PointLight*> pointLights() {
      return _pointLights;
    }

    inline std::vector<Spotlight*> spotlights() {
      return _spotlights;
    }

  private:
  
    ComponentReference<Camera> _camera;

    Vector2 _screenSize;

    Matrix _screenProjection;
    
    Vector _cameraPosition;
    
    Matrix _viewProjection;

    std::vector<DirectionalLight*> _dirLights;

    std::vector<PointLight*> _pointLights;

    std::vector<Spotlight*> _spotlights;
  };

}

#endif //RENDER_CONTEXT_H