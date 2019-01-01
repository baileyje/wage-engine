#ifndef ENTITY_CAMERA_H
#define ENTITY_CAMERA_H

#include "entity/component.h"
#include "math/frustum.h"
#include "entity/component/reference.h"

namespace wage {

  enum CameraType { perspective, orthographic };

  class Camera : public Component  {

  public:

    static ComponentReference<Camera> main;

    Camera(CameraType type) : Component("Camera"), type(type) {    
    }

    ~Camera() {    
    }

    inline CameraType getType() {
      return type;
    }

    virtual Matrix screenProjection(Vector2 screenSize) = 0;

    Matrix viewProjection() {
      Transform* transform = getTransform();
      Vector camPos = transform->getPosition();
      Quaternion rotation = transform->getRotation();
      Vector camFront = rotation * Vector(0, 0, 1);
      Vector camUp = rotation * Vector(0, 1, 0);
      return glm::lookAt(camPos, camPos + camFront, camUp);
    }

    virtual Frustum frustum(Vector2 screenSize) = 0;

  private:

    CameraType type;

  };

}

#endif //ENTITY_CAMERA_H