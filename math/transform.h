#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"


namespace wage {

  class Transform {

  public:

    Transform() : parent(nullptr), position(0, 0, 0), scale(1, 1, 1), rotation(Vector(0,0,0)) {}

    ~Transform() {}

    inline Vector getLocalPosition() { return position; }

    inline void setLocalPosition(Vector position) { this->position = position; };

    inline Vector getPosition() { 
      if (parent) {
        return parent->getPosition() + position;
      }
      return position; 
    }

    inline void setPosition(Vector position) { 
      if (parent) {
        this->position = position - parent->getPosition();
        return;
      }   
      this->position = position;
    }

    inline Vector getLocalScale() { 
      return scale; 
    }

    inline Vector getScale() { 
      if (parent) {
        Vector parentScale = parent->getScale();
        return Vector(
          parentScale.x * scale.x, 
          parentScale.y * scale.y, 
          parentScale.z * scale.z 
        );
      }    
      return scale; 
    }

    inline void setLocalScale(Vector scale) { this->scale = scale; };

    inline Quaternion getLocalRotation() { 
      return rotation; 
    }

    inline Quaternion getRotation() { 
      if (parent) {
        Quaternion parentQuat = parent->getRotation();
        return parentQuat * this->rotation;      
      }
      return rotation; 
    }

    inline void setLocalRotation(Quaternion rotation) {    
      this->rotation = rotation; 
    }

    inline void setLocalRotation(Vector eulers) { 
      this->rotation = quatFromEulers(eulers);
    }

    inline void setRotation(Vector eulers) { 
      setRotation(quatFromEulers(eulers));
    }

    inline void setRotation(Quaternion rotation) { 
      if (parent) {
        Quaternion parentQuat = parent->getRotation();
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

    Quaternion rotation;

  };

}

#endif //MATH_TRANSFORM_H