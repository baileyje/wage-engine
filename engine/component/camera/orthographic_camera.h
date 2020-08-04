#pragma once

#include "component/camera/camera.h"

namespace wage { namespace component {

  class OrthographicCamera : public Camera {

  public:
    OrthographicCamera() : OrthographicCamera(0, 800, 0, 600, -10.0f, 100.0f) {
    }

    OrthographicCamera(float left, float right, float top, float bottom, float nearClip, float farClip)
        : Camera(CameraType::orthographic), _left(left), _right(right), _top(top), _bottom(bottom), _nearClip(nearClip), _farClip(farClip) {
    }

    inline float left() const {
      return _left;
    }

    inline void left(float left) {
      _left = left;
    }

    inline float right() const {
      return _right;
    }

    inline void right(float right) {
      _right = right;
    }

    inline float top() const {
      return _top;
    }

    inline void top(float top) {
      _top = top;
    }

    inline float bottom() const {
      return _bottom;
    }

    inline void bottom(float bottom) {
      _bottom = bottom;
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

    void farClip(float farClip) {
      _farClip = farClip;
    }

    inline math::Matrix screenProjection(math::Vector2 screenSize) const {
      return math::Matrix::orthographic(
          left(), right(),
          top(), bottom(),
          nearClip(), farClip());
    }

    inline math::Frustum frustum(math::Vector2 screenSize, math::Transform* cameraTransform) const {
      math::Frustum returnFrustum;
      return returnFrustum;
    }

  private:
    float _left;

    float _right;

    float _top;

    float _bottom;

    float _nearClip;

    float _farClip;
  };

} }