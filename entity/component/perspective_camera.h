#ifndef ENTITY_PERSPECTIVE_CAMERA_H
#define ENTITY_PERSPECTIVE_CAMERA_H

#include "entity/component/camera.h"

class PerspectiveCamera : public Camera  {

public:

  PerspectiveCamera() : PerspectiveCamera(60, 0.2, 1000.0) {
  }

  PerspectiveCamera(float fov, float nearClip, float farClip) 
    : Camera(perspective), fov(fov), nearClip(nearClip), farClip(farClip) {
  }

  inline float getFov() {
    return fov;
  }

  inline void setFov(float fov) {
    this->fov = fov;
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

private:

  float fov;

  float nearClip;

  float farClip;

};

#endif //ENTITY_PERSPECTIVE_CAMERA_H