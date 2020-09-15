#include "math/vector.h"
#include <limits>
#include <iostream>

namespace wage {
  namespace math {

    Vector3 Vector3::Zero = {0, 0, 0};

    Vector3 Vector3::One = {1, 1, 1};

    Vector3 Vector3::Up = {0, 1, 0};

    Vector3 Vector3::Down = {0, -1, 0};

    Vector3 Vector3::Forward = {0, 0, 1};

    Vector3 Vector3::Backward = {0, 0, -1};

    Vector3 Vector3::Left = {-1, 0, 0};

    Vector3 Vector3::Right = {1, 0, 0};

    Vector2 Vector2::Zero = {0, 0};

    Vector2 Vector2::One = {1, 1};

    Vector2 Vector2::Invalid = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    Vector3 Vector3::Invalid = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    Vector3::Vector3(const Vector2& vector) : Vector3(vector.x, vector.y, 0) {
    }

    Vector3 Vector3::cross(const Vector3& left, const Vector3& right) {
      return glm::cross(left.glm(), right.glm());
    }

    std::ostream& operator<<(std::ostream& os, Vector3 vec) {
      os << "vec3(" << vec.x << ':' << vec.y << ':' << vec.z << ")";
      return os;
    }

    std::ostream& operator<<(std::ostream& os, Vector2 vec) {
      os << "vec2(" << vec.x << ':' << vec.y << ")";
      return os;
    }
  }
}