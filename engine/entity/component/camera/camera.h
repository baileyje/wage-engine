#ifndef ENTITY_CAMERA_H
#define ENTITY_CAMERA_H

#include "entity/component.h"
#include "math/frustum.h"
#include "memory/reference.h"

namespace wage {

  enum class CameraType { Perspective, Orthographic };

  class Camera : public Component  {

  public:

    Camera(CameraType type) : Component("Camera"), _type(type) {    
    }

    ~Camera() {    
    }

    inline CameraType type() {
      return _type;
    }

    virtual Matrix screenProjection(Vector2 screenSize) = 0;

    Matrix viewProjection(Transform* trans) {
      Vector camPos = trans->position();
      Quaternion rotation = trans->rotation();
      Vector camFront = rotation * Vector(0, 0, 1);
      Vector camUp = rotation * Vector(0, 1, 0);
      return glm::lookAt(camPos, camPos + camFront, camUp);
    }

    virtual Frustum frustum(Vector2 screenSize, Transform* cameraTransform) = 0;

  private:

    CameraType _type;

  };

}

#endif //ENTITY_CAMERA_H