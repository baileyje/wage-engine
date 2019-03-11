#ifndef ENTITY_CAMERA_H
#define ENTITY_CAMERA_H

#include "math/frustum.h"
#include "math/matrix.h"
#include "math/transform.h"
#include "memory/reference.h"

namespace wage {

  enum class CameraType { perspective,
                          orthographic };

  class Camera {

  public:
    Camera(CameraType type) : _type(type) {
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
      Vector camFront = rotation * Vector::Forward;
      Vector camUp = rotation * Vector::Up;
      return Matrix::lookAt(camPos, camPos + camFront, camUp);
    }

    virtual Frustum frustum(Vector2 screenSize, Transform* cameraTransform) = 0;

  private:
    CameraType _type;
  };
}

#endif //ENTITY_CAMERA_H