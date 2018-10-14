#ifndef ENTITY_TRANSFORM_H
#define ENTITY_TRANSFORM_H

#include "entity/vector.h"
#include "entity/quaternion.h"


class Transform {

public:

  Transform() {}

  ~Transform() {}

  Vector* getPosition() { return &position; }

  void setPosition(Vector position) { this->position = position; };

  Vector* getScale() { return &scale; }

  void setScale(Vector scale) { this->scale = scale; };

  Vector* getRotation() { return &rotation; }

  void setRotation(Vector rotation) { this->rotation = rotation; }

private:

  Vector position;
  
  Vector scale;

  // TODO: Quaternion?
  Vector rotation;

};

#endif //ENTITY_TRANSFORM_H