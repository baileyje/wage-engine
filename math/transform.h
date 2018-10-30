#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"


class Transform {

public:

  Transform() : parent(nullptr), position(0, 0, 0), scale(1, 1, 1), rotation(Vector(0,0,0)) {}

  ~Transform() {}

  inline Vector getPosition() { return position; }

  inline void setPosition(Vector position) { this->position = position; };

  inline Vector getWorldPosition() { 
    if (parent) {
      return parent->getWorldPosition() + position;
    }
    return position; 
  }

  inline void setWorldPosition(Vector position) { 
    if (parent) {
      this->position = position - parent->getWorldPosition();
      return;
    }   
    this->position = position;
  }

  inline Vector getScale() { 
    return scale; 
  }

  inline Vector getWorldScale() { 
    if (parent) {
      Vector parentScale = parent->getWorldScale();
      return Vector(
        parentScale.x * scale.x, 
        parentScale.y * scale.y, 
        parentScale.z * scale.z 
      );
    }    
    return scale; 
  }

  inline void setScale(Vector scale) { this->scale = scale; };

  inline Quaternion getRotation() { 
    return rotation; 
  }

  inline Quaternion getWorldRotation() { 
    if (parent) {
      Quaternion parentQuat = parent->getWorldRotation();
      return parentQuat * this->rotation;      
    }
    return rotation; 
  }

  inline void setRotation(Quaternion rotation) {    
    this->rotation = rotation; 
  }

  inline void setRotation(Vector eulers) { 
    this->rotation = quatFromEulers(eulers);
  }

  inline void setWorldRotation(Quaternion rotation) { 
    if (parent) {
      Quaternion parentQuat = parent->getWorldRotation();
      this->rotation = rotation * Quaternion(
        parentQuat.w,
        -parentQuat.x,
        -parentQuat.y,
        -parentQuat.z        
      );   
    } else {
      this->rotation = rotation;
    }        
  }

  inline Matrix localPorjection() {
    Matrix translation = glm::translate(glm::mat4(1), position);
    Matrix scale = glm::scale(glm::mat4(1), this->scale);
    Matrix rotate = glm::toMat4(this->rotation);
    return translation * rotate * scale;
  }

  inline Matrix worldPorjection() {
    Matrix local = localPorjection();
    if (parent) {
      Matrix parentProjection = parent->worldPorjection();
      return parentProjection * local;
    }
    return local;
  }

  inline Transform* getParent() { 
    return parent;
  }

  inline void setParent(Transform* parent) { 
    this->parent = parent;
  }

private:

  Transform* parent;

  Vector position;
  
  Vector scale;

  // TODO: Quaternion?
  Quaternion rotation;
};

#endif //MATH_TRANSFORM_H