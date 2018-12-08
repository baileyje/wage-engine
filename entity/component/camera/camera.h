#ifndef ENTITY_CAMERA_H
#define ENTITY_CAMERA_H

#include "entity/component.h"

namespace wage {

  enum CameraType { perspective, orthographic };

  class Camera : public Component  {

  public:

    Camera(CameraType type) : Component("Camera"), type(type) {    
    }

    ~Camera() {    
    }

    inline CameraType getType() {
      return type;
    }

    virtual Matrix screenProjection(Vector2 screenSize) = 0;

    static Matrix viewProjectionFor(Transform& transform) {
      Vector camPos = transform.getPosition();
      Quaternion rotation = transform.getRotation();
      Vector camFront(
        2 * (rotation.x * rotation.z + rotation.w * rotation.y), 
        2 * (rotation.y * rotation.z - rotation.w * rotation.x),
        1 - 2 * (rotation.x * rotation.x + rotation.y * rotation.y)
      );
      Vector camUp(
        2 * (rotation.x * rotation.y - rotation.w * rotation.z), 
        1 - 2 * (rotation.x * rotation.x - rotation.z * rotation.z),
        2 * (rotation.y * rotation.z + rotation.w * rotation.x)
      );
      return  glm::lookAt(camPos, camPos + camFront, camUp);
    }

  private:

    CameraType type;

  };

}

#endif //ENTITY_CAMERA_H