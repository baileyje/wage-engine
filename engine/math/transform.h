#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"


namespace wage {

  class Transform {

  public:

    Transform() : parent_(nullptr), position_(0, 0, 0), scale_(1, 1, 1), rotation_(Vector(0,0,0)) {}

    ~Transform() {}

    inline Vector& localPosition() { return position_; }

    inline void setLocalPosition(Vector position) { position_ = position; };

    inline Vector position() { 
      if (parent_) {
        return parent_->position() + position_;
      }
      return position_; 
    }

    inline void setPosition(Vector position) { 
      if (parent_) {
        position_ = position - parent_->position();
        return;
      }   
      position_ = position;
    }

    inline Vector& localScale() { 
      return scale_; 
    }

    inline Vector scale() { 
      if (parent_) {
        Vector parentScale = parent_->scale();
        return Vector(
          parentScale.x * scale_.x, 
          parentScale.y * scale_.y, 
          parentScale.z * scale_.z 
        );
      }    
      return scale_; 
    }

    inline void setLocalScale(Vector scale) { scale_ = scale; };

    inline Quaternion& localRotation() { 
      return rotation_; 
    }

    inline Quaternion rotation() { 
      if (parent_) {
        Quaternion parentQuat = parent_->rotation();
        return parentQuat * rotation_;
      }
      return rotation_; 
    }

    inline void setLocalRotation(Quaternion rotation) {    
      rotation_ = rotation; 
    }

    inline void setLocalRotation(Vector eulers) { 
      rotation_ = quatFromEulers(eulers);
    }

    inline void setRotation(Vector eulers) { 
      setRotation(quatFromEulers(eulers));
    }

    inline void setRotation(Quaternion rotation) { 
      if (parent_) {
        Quaternion parentQuat = parent_->rotation();
        rotation_ = rotation * Quaternion(
          parentQuat.w,
          -parentQuat.x,
          -parentQuat.y,
          -parentQuat.z        
        );   
      } else {
        rotation_ = rotation;
      }        
    }

    inline Matrix localPorjection() {
      Matrix translation = glm::translate(glm::mat4(1), position_);
      Matrix scale = glm::scale(glm::mat4(1), scale_);
      Matrix rotate = glm::toMat4(rotation_);
      return translation * rotate * scale;
    }

    inline Matrix worldPorjection() {
      Matrix local = localPorjection();
      if (parent_) {
        Matrix parentProjection = parent_->worldPorjection();
        return parentProjection * local;
      }
      return local;
    }

    inline Transform* parent() { 
      return parent_;
    }

    inline void setParent(Transform* parent) { 
      parent_ = parent;
    }

  private:

    Transform* parent_;

    Vector position_;
    
    Vector scale_;

    Quaternion rotation_;

  };

}

#endif //MATH_TRANSFORM_H