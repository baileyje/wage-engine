#ifndef ENTITY_TRANSFORM_H
#define ENTITY_TRANSFORM_H

#include "entity/vector.h"
#include "entity/quaternion.h"


class Transform {

public:

  Transform() : scale(Vector(1, 1, 1)) {}

  ~Transform() {}

  Vector* getPosition() { return &position; }

  void setPosition(Vector position) { this->position = position; };

  Vector* getScale() { return &scale; }

  void setScale(Vector scale) { this->scale = scale; };

  Vector* getRotation() { return &rotation; }

  void setRotation(Vector rotation) { this->rotation = rotation; }

  Vector position;
  
  Vector scale;

  // TODO: Quaternion?
  Vector rotation;

private:



};

#endif //ENTITY_TRANSFORM_H