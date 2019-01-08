#ifndef ENTITY_ORTHOGRAPHIC_CAMERA_H
#define ENTITY_ORTHOGRAPHIC_CAMERA_H

#include "entity/component/camera/camera.h"

namespace wage {

  class OrthographicCamera : public Camera  {

  public:

    OrthographicCamera() : OrthographicCamera(0, 800, 0, 600,  -10.0f, 100.0f) {
    }

    OrthographicCamera(float left, float right, float top, float bottom, float nearClip, float farClip) 
      : Camera(orthographic), left(left), right(right), top(top), bottom(bottom), nearClip(nearClip), farClip(farClip) {
    }

    inline float getLeft() {
      return left;
    }

    inline void setLeft(float left) {
      this->left = left;
    }

    inline float getRight() {
      return right;
    }

    inline void setRight(float right) {
      this->right = right;
    }

    inline float getTop() {
      return top;
    }

    inline void setTop(float top) {
      this->top = top;
    }

    inline float getBottom() {
      return bottom;
    }

    inline void setBottom(float bottom) {
      this->bottom = bottom;
    }

    inline float getNearClip() {
      return nearClip;
    }

    inline void setNearClip(float nearClip) {
      this->nearClip = nearClip;
    }

    inline float getFarClip() {
      return farClip;
    }

    void setFarClip(float farClip) {
      this->farClip = farClip;
    }

    inline Matrix screenProjection(Vector2 screenSize) {
      return glm::ortho(
        getLeft(), getRight(),
        getTop(), getBottom(),
        getNearClip(), getFarClip()
      );
    }

    

    
    

  private:

    float left;

    float right;

    float top;

    float bottom;

    float nearClip;

    float farClip;

  };

}

#endif //ENTITY_ORTHOGRAPHIC_CAMERA_H