#pragma once

#include "math/frustum.h"
#include "math/matrix.h"
#include "math/transform.h"
#include "memory/reference.h"

namespace wage { namespace component {

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

    virtual math::Matrix screenProjection(math::Vector2 screenSize) const = 0;

    math::Matrix viewProjection(math::Transform* trans) const {
      math::Vector camPos = trans->position();
      math::Quaternion rotation = trans->rotation();
      math::Vector camFront = rotation * math::Vector::Forward;
      math::Vector camUp = rotation * math::Vector::Up;
      return math::Matrix::lookAt(camPos, camPos + camFront, camUp);
    }

    virtual math::Frustum frustum(math::Vector2 screenSize, math::Transform* cameraTransform) const = 0;

  private:
    CameraType _type;
  };

} }