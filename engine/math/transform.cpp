#include "math/transform.h"

namespace wage::math {
  Transform::Transform(Vector position, Vector scale, Vector rotation) : localPosition(position), localScale(scale), localRotation(Quaternion::fromEulers(rotation)) {}

  Transform::Transform(Vector position, Vector scale, Quaternion rotation) : localPosition(position), localScale(scale), localRotation(rotation) {}

  Transform::Transform() : localPosition(Vector::Zero), localScale(Vector::One), localRotation(Vector::Zero) {}

  Transform::~Transform() {}

  Vector Transform::position(Vector3 parentPosition) const {
    return parentPosition + localPosition;
  }

  Vector3 Transform::scale(Vector3 parentScale) const {
    return Vector(
      parentScale.x * localScale.x,
      parentScale.y * localScale.y,
      parentScale.z * localScale.z
    );
  }

  Quaternion Transform::rotation(Quaternion parentQuat) const {
    return localRotation * parentQuat;
  }

  void Transform::setEulerRotation(Vector3 eulers) {
    localRotation = Quaternion::fromEulers(eulers);
  }

  Matrix Transform::localProjection() const {
    Matrix translation = Matrix(1).translate(localPosition);
    Matrix scale = Matrix(1).scale(localScale);
    Matrix rotate = Matrix(localRotation);
    return translation * rotate * scale;
  }

  Matrix Transform::worldProjection(Transform parent) const {
    return parent.localProjection() * localProjection();
  }
}