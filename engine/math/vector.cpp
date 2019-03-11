#include "math/vector.h"

namespace wage {

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

  Vector3::Vector3(const Vector2& vector) : Vector3(vector.x, vector.y, 0) {
  }
}