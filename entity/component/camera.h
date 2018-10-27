#ifndef ENTITY_CAMERA_H
#define ENTITY_CAMERA_H

#include "entity/component.h"

enum CameraType { perspective, orthographic };

class Camera : public Component  {

public:

  Camera(CameraType type) : Component("Camera"), type(type) {    
  }

  ~Camera() {    
  }

  inline CameraType getType() {
    return type;
  }

private:

  CameraType type;

};

#endif //ENTITY_CAMERA_H