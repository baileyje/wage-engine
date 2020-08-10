#pragma once

#include "component/camera/camera.h"

#define PerspectiveCameraComponent 12

namespace wage {
  namespace component {

    class PerspectiveCamera : public Camera {

    public:
      PerspectiveCamera() : PerspectiveCamera(60, 0.2, 10000.0) {
      }

      PerspectiveCamera(float fov, float nearClip, float farClip)
          : Camera(CameraType::perspective), _fov(fov), _nearClip(nearClip), _farClip(farClip) {
      }

      inline float fov() const {
        return _fov;
      }

      inline void fov(float fov) {
        _fov = fov;
      }

      inline float nearClip() const {
        return _nearClip;
      }

      inline void nearClip(float nearClip) {
        _nearClip = nearClip;
      }

      inline float farClip() const {
        return _farClip;
      }

      inline void farClip(float farClip) {
        _farClip = farClip;
      }

      inline math::Matrix screenProjection(math::Vector2 screenSize) const {
        return math::Matrix::perspective(glm::radians(fov()), screenSize.x / screenSize.y, nearClip(), farClip());
      }

      inline math::Frustum frustum(math::Vector2 screenSize, math::Transform* cameraTransform) const {
        math::Frustum returnFrustum;
        math::Matrix mvp = screenProjection(screenSize) * viewProjection(cameraTransform);
        returnFrustum.extractPlanesFrom(mvp);
        return returnFrustum;
      }

    private:
      float _fov;

      float _nearClip;

      float _farClip;
    };

  }
}