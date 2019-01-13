#ifndef ENTITY_PERSPECTIVE_CAMERA_H
#define ENTITY_PERSPECTIVE_CAMERA_H

#include "entity/component/camera/camera.h"

namespace wage {

  class PerspectiveCamera : public Camera  {

  public:

    PerspectiveCamera() : PerspectiveCamera(60, 0.2, 1000.0) {
    }

    PerspectiveCamera(float fov, float nearClip, float farClip) 
      : Camera(CameraType::perspective), fov_(fov), nearClip_(nearClip), farClip_(farClip) {
    }

    inline float fov() {
      return fov_;
    }

    inline void setFov(float fov) {
      fov_ = fov;
    }

    inline float nearClip() {
      return nearClip_;
    }

    inline void setNearClip(float nearClip) {
      nearClip_ = nearClip;
    }

    inline float farClip() {
      return farClip_;
    }

    inline void setFarClip(float farClip) {
      farClip_ = farClip;
    }

    inline Matrix screenProjection(Vector2 screenSize) {
      return glm::perspective(glm::radians(fov()), screenSize.x / screenSize.y, nearClip(), farClip());
    }

    inline Frustum frustum(Vector2 screenSize) {
      Frustum returnFrustum;
      Matrix mvp =  screenProjection(screenSize) * viewProjection();
      returnFrustum.extractPlanesFrom(mvp);
      return returnFrustum;
    }


  private:

    float fov_;

    float nearClip_;

    float farClip_;

  };

}

#endif //ENTITY_PERSPECTIVE_CAMERA_H