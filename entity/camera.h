#ifndef RENDERER_CAMERA_H
#define RENDERER_CAMERA_H

#include "entity/transform.h"

class Camera {

public:
  
  Camera() {}

  Camera(Transform transform) : transform(transform) {    
  }

  ~Camera() {}

  Transform* getTransform() {
    return &transform;
  }

private:

  Transform transform;

};

#endif //RENDERER_CAMERA_H