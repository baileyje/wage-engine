#ifndef ENTITY_ORTHOGRAPHIC_CAMERA_H
#define ENTITY_ORTHOGRAPHIC_CAMERA_H

#include "component/camera/camera.h"

namespace wage {

  class OrthographicCamera : public Camera  {

  public:

    OrthographicCamera() : OrthographicCamera(0, 800, 0, 600,  -10.0f, 100.0f) {
    }

    OrthographicCamera(float left, float right, float top, float bottom, float nearClip, float farClip) 
      : Camera(CameraType::Orthographic), _left(left), _right(right), _top(top), _bottom(bottom), _nearClip(nearClip), _farClip(farClip) {
    }

    inline float left() {
      return _left;
    }

    inline void left(float left) {
      _left = left;
    }

    inline float right() {
      return _right;
    }

    inline void right(float right) {
      _right = right;
    }

    inline float top() {
      return _top;
    }

    inline void top(float top) {
      _top = top;
    }

    inline float bottom() {
      return _bottom;
    }

    inline void bottom(float bottom) {
      _bottom = bottom;
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

    void farClip(float farClip) {
      _farClip = farClip;
    }

    inline Matrix screenProjection(Vector2 screenSize) {
      return Matrix::orthographic(
        left(), right(),
        top(), bottom(),
        nearClip(), farClip()
      );
    }

    inline Frustum frustum(Vector2 screenSize, Transform* cameraTransform) {
      Frustum returnFrustum;
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

}

#endif //ENTITY_ORTHOGRAPHIC_CAMERA_H