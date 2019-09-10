#include "math/quaternion.h"

namespace wage { namespace math {

  glm::quat wrap(Quaternion quat) {
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
  }

  Quaternion::Quaternion(Vector vector) {
    glm::quat tmp = glm::quat({vector.x, vector.y, vector.z});
    x = tmp.x;
    y = tmp.y;
    z = tmp.z;
    w = tmp.w;
  }

  Quaternion Quaternion::fromEulers(const Vector& vector) {
    return Quaternion({glm::radians(vector.x),
                       glm::radians(vector.y),
                       glm::radians(vector.z)});
  }

  Vector Quaternion::eulerAngles() const {
    return Vector(glm::eulerAngles(wrap(*this)));
  }

  Quaternion Quaternion::rotated(float angle, Vector axis) const {
    auto rotated = glm::rotate(wrap(*this), angle, axis.glm());
    return Quaternion(rotated.x, rotated.y, rotated.z, rotated.w);
  }

  Quaternion& Quaternion::operator*=(const Quaternion& rhs) {
    glm::quat leftTmp = wrap(*this);
    glm::quat rightTmp = wrap(rhs);
    leftTmp *= rightTmp;
    x = leftTmp.x;
    y = leftTmp.y;
    z = leftTmp.z;
    w = leftTmp.w;
    return *this;
  }

  Vector operator*(Quaternion quat, const Vector& vector) {
    glm::vec3 tmp = wrap(quat) * vector.glm();
    return Vector(tmp.x, tmp.y, tmp.z);
    ;
  }

} }