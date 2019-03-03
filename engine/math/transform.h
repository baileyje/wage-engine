#ifndef MATH_TRANSFORM_H
#define MATH_TRANSFORM_H

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"


namespace wage {

  class Transform {

  public:

    Transform() : _parent(nullptr), _position(Vector::Zero), _scale(Vector::One), _rotation(Vector::Zero) {}

    ~Transform() {}

    inline Vector& localPosition() { return _position; }

    inline void localPosition(Vector position) { _position = position; };

    inline Vector position() { 
      if (_parent) {
        return _parent->position() + _position;
      }
      return _position; 
    }

    inline void position(Vector position) { 
      if (_parent) {
        _position = position - _parent->position();
        return;
      }   
      _position = position;
    }

    inline Vector& localScale() { 
      return _scale; 
    }

    inline Vector scale() { 
      if (_parent) {
        Vector parentScale = _parent->scale();
        return Vector(
          parentScale.x * _scale.x, 
          parentScale.y * _scale.y, 
          parentScale.z * _scale.z 
        );
      }    
      return _scale; 
    }

    inline void localScale(Vector scale) { _scale = scale; };

    inline Quaternion& localRotation() { 
      return _rotation; 
    }

    inline Quaternion rotation() { 
      if (_parent) {
        Quaternion parentQuat = _parent->rotation();
        return parentQuat * _rotation;
      }
      return _rotation; 
    }

    inline void localRotation(Quaternion rotation) {    
      _rotation = rotation; 
    }

    inline void localRotation(Vector eulers) { 
      _rotation = Quaternion::fromEulers(eulers);
    }

    inline void rotation(Vector eulers) { 
      rotation(Quaternion::fromEulers(eulers));
    }

    inline void rotation(Quaternion rotation) { 
      if (_parent) {
        Quaternion parentQuat = _parent->rotation();
        _rotation = rotation * Quaternion(
          parentQuat.w,
          -parentQuat.x,
          -parentQuat.y,
          -parentQuat.z        
        );   
      } else {
        _rotation = rotation;
      }        
    }

    inline Matrix localPorjection() {
      Matrix translation = Matrix(1).translate(_position);
      Matrix scale = Matrix(1).scale(_scale);
      Matrix rotate = Matrix(_rotation);
      return translation * rotate * scale;
    }

    inline Matrix worldPorjection() {
      Matrix local = localPorjection();
      if (_parent) {
        Matrix parentProjection = _parent->worldPorjection();
        return parentProjection * local;
      }
      return local;
    }

    inline Transform* parent() { 
      return _parent;
    }

    inline void parent(Transform* parent) { 
      _parent = parent;
    }

  private:

    Transform* _parent;

    Vector _position;
    
    Vector _scale;

    Quaternion _rotation;

  };

}

#endif //MATH_TRANSFORM_H