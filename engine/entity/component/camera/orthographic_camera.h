#ifndef ENTITY_ORTHOGRAPHIC_CAMERA_H
#define ENTITY_ORTHOGRAPHIC_CAMERA_H

#include "entity/component/camera/camera.h"

namespace wage {

  class OrthographicCamera : public Camera  {

  public:

    OrthographicCamera() : OrthographicCamera(0, 800, 0, 600,  -10.0f, 100.0f) {
    }

    OrthographicCamera(float left, float right, float top, float bottom, float nearClip, float farClip) 
      : Camera(orthographic), left_(left), right_(right), top_(top), bottom_(bottom), nearClip_(nearClip), farClip_(farClip) {
    }

    inline float left() {
      return left_;
    }

    inline void left(float left) {
      left_ = left;
    }

    inline float right() {
      return right_;
    }

    inline void right(float right) {
      right_ = right;
    }

    inline float top() {
      return top_;
    }

    inline void top(float top) {
      top_ = top;
    }

    inline float bottom() {
      return bottom_;
    }

    inline void bottom(float bottom) {
      bottom_ = bottom;
    }

    inline float nearClip() {
      return nearClip_;
    }

    inline void nearClip(float nearClip) {
      nearClip_ = nearClip;
    }

    inline float farClip() {
      return farClip_;
    }

    void farClip(float farClip) {
      farClip_ = farClip;
    }

    inline Matrix screenProjection(Vector2 screenSize) {
      return glm::ortho(
        getLeft(), getRight(),
        getTop(), getBottom(),
        getNearClip(), getFarClip()
      );
    }

    

    
    

  private:

    float left_;

    float right_;

    float top_;

    float bottom_;

    float nearClip_;

    float farClip_;

  };

}

#endif //ENTITY_ORTHOGRAPHIC_CAMERA_H