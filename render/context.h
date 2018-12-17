#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <vector>

#include "render/gl/texture_manager.h"
#include "render/gl/vao_manager.h"

#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"
#include "entity/component/camera/camera.h"

namespace wage {

  class RenderContext {

  public:

    RenderContext(Camera* camera, Vector2 screenSize,
      std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights, std::vector<Spotlight*> spotlights) 
      : camera_(camera), screenSize_(screenSize),
      dirLights_(dirLights), pointLights_(pointLights), spotlights_(spotlights) {
          screenProjection_ = camera->screenProjection(screenSize);
          viewProjection_ = camera->viewProjection();
          cameraPosition_ = camera->getTransform()->getPosition();
      }

    inline Camera* camera() {
      return camera_;
    }

    inline Vector2 screenSize() {
      return screenSize_;
    }

    inline Matrix screenProjection() {
      return screenProjection_;
    }
    
    inline Vector cameraPosition() {
      return cameraPosition_;
    }
    
    inline Matrix viewProjection() {
      return viewProjection_;
    }

    inline std::vector<DirectionalLight*> dirLights() {
      return dirLights_;
    }

    inline std::vector<PointLight*> pointLights() {
      return pointLights_;
    }

    inline std::vector<Spotlight*> spotlights() {
      return spotlights_;
    }

  private:
  
    Camera* camera_;

    Vector2 screenSize_;

    Matrix screenProjection_;
    
    Vector cameraPosition_;
    
    Matrix viewProjection_;

    std::vector<DirectionalLight*> dirLights_;

    std::vector<PointLight*> pointLights_;

    std::vector<Spotlight*> spotlights_;
  };

}

#endif //RENDER_CONTEXT_H