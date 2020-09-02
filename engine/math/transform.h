#pragma once

#include "math/vector.h"
#include "math/matrix.h"
#include "math/quaternion.h"

#define TransformComponent 0

namespace wage {
  namespace math {

    class Transform {

    public:
      Transform(Vector position, Vector scale, Vector rotation) : _parent(nullptr), _position(position), _scale(scale), _rotation(Quaternion::fromEulers(rotation)) {}

      Transform(Vector position, Vector scale, Quaternion rotation) : _parent(nullptr), _position(position), _scale(scale), _rotation(rotation) {}

      Transform() : _parent(nullptr), _position(Vector::Zero), _scale(Vector::One), _rotation(Vector::Zero) {}

      ~Transform() {}

      inline Vector& localPosition() { return _position; }

      inline void localPosition(Vector position) { _position = position; };

      inline Vector position() {
        if (_parent) {
          return _parent->position() + _position;
        }
        return localPosition();
      }

      inline void position(Vector position) {
        if (_parent) {
          _position = position - _parent->position();
          return;
        }
        localPosition(position);
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
              parentScale.z * _scale.z);
        }
        return localScale();
      }

      inline void localScale(Vector scale) { _scale = scale; };

      inline Quaternion& localRotation() {
        return _rotation;
      }

      inline Quaternion rotation() {
        if (_parent) {
          Quaternion parentQuat = _parent->rotation();
          return _rotation * parentQuat;
        }
        return localRotation();
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
                                     -parentQuat.z);
        } else {
          localRotation(rotation);
        }
      }

      inline Matrix localProjection() {
        Matrix translation = Matrix(1).translate(_position);
        Matrix scale = Matrix(1).scale(_scale);
        Matrix rotate = Matrix(_rotation);
        return translation * rotate * scale;
      }

      inline Matrix worldProjection() {
        Matrix local = localProjection();
        if (_parent) {
          Matrix parentProjection = _parent->worldProjection();
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
}
