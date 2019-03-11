#ifndef ENTITY_PERSPECTIVE_CAMERA_H
#define ENTITY_PERSPECTIVE_CAMERA_H

#include "component/camera/camera.h"

namespace wage {

  class PerspectiveCamera : public Camera {

  public:
    PerspectiveCamera() : PerspectiveCamera(60, 0.2, 1000.0) {
    }

    PerspectiveCamera(float fov, float nearClip, float farClip)
        : Camera(CameraType::perspective), _fov(fov), _nearClip(nearClip), _farClip(farClip) {
    }

    inline float fov() {
      return _fov;
    }

    inline void fov(float fov) {
      _fov = fov;
    }

    inline float nearClip() {
      return _nearClip;
    }

    inline void nearClip(float nearClip) {
      _nearClip = nearClip;
    }

    inline float farClip() {
      return _farClip;
    }

    inline void farClip(float farClip) {
      _farClip = farClip;
    }

    inline Matrix screenProjection(Vector2 screenSize) {
      return Matrix::perspective(glm::radians(fov()), screenSize.x / screenSize.y, nearClip(), farClip());
    }

    inline Frustum frustum(Vector2 screenSize, Transform* cameraTransform) {
      Frustum returnFrustum;
      Matrix mvp = screenProjection(screenSize) * viewProjection(cameraTransform);
      returnFrustum.extractPlanesFrom(mvp);
      return returnFrustum;
    }

  private:
    float _fov;

    float _nearClip;

    float _farClip;
  };
}

#endif //ENTITY_PERSPECTIVE_CAMERA_H