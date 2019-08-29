#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "glm/ext.hpp"

namespace wage {

  class Vector2;

  class Vector3 {

  public:
    float x;

    float y;

    float z;

    static Vector3 Zero;

    static Vector3 One;

    static Vector3 Up;

    static Vector3 Down;

    static Vector3 Forward;

    static Vector3 Backward;

    static Vector3 Left;

    static Vector3 Right;

    Vector3() : Vector3(0) {
    }

    Vector3(float v) : Vector3(v, v, v) {
    }

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {
    }

    Vector3(const Vector2& vector);

    // TODO: Remove!
    Vector3(glm::vec3 vec) : Vector3(vec.x, vec.y, vec.z) {
    }

    // Operators
    Vector3& operator+=(const Vector3& rhs) {
      this->x += rhs.x;
      this->y += rhs.y;
      this->z += rhs.z;
      return *this;
    }

    Vector3& operator-=(const Vector3& rhs) {
      this->x -= rhs.x;
      this->y -= rhs.y;
      this->z -= rhs.z;
      return *this;
    }

    Vector3& operator*=(const Vector3& rhs) {
      this->x *= rhs.x;
      this->y *= rhs.y;
      this->z *= rhs.z;
      return *this;
    }

    Vector3& operator*=(const float& val) {
      this->x *= val;
      this->y *= val;
      this->z *= val;
      return *this;
    }

    Vector3& operator/=(const float& val) {
      this->x /= val;
      this->y /= val;
      this->z /= val;
      return *this;
    }

    friend Vector3 operator+(Vector3 lhs, const Vector3& rhs) {
      lhs += rhs;
      return lhs;
    }

    friend Vector3 operator-(Vector3 lhs, const Vector3& rhs) {
      lhs -= rhs;
      return lhs;
    }

    friend Vector3 operator*(Vector3 lhs, const Vector3& rhs) {
      lhs *= rhs;
      return lhs;
    }

    friend Vector3 operator*(Vector3 lhs, const float& val) {
      lhs *= val;
      return lhs;
    }

    friend Vector3 operator/(Vector3 lhs, const float& val) {
      lhs /= val;
      return lhs;
    }

    static float dot(const Vector3& left, const Vector3& right) {
      Vector3 tmp = left * right;
      return tmp.x + tmp.y + tmp.z;
    }

    static Vector3 cross(const Vector3& left, const Vector3& right);

    static float distance(const Vector3& left, const Vector3& right) {
      return left.distanceTo(right);
    }

    float distanceTo(const Vector3& other) const {
      auto dist = sqrtf(
          powf(x - other.x, 2) +
          powf(y - other.y, 2) +
          powf(z - other.z, 2));
      return dist;
    }

    Vector3 normalized() const {
      return *this / magnatude();
    }

    float magnatude() const {
      return Vector3::distance(Zero, *this);
    }

    // TODO: REMOVE THIS JUNK!
    glm::vec3 glm() const {
      return glm::vec3(x, y, z);
    }
  };

  class Vector2 {

  public:
    float x;

    float y;

    static Vector2 Zero;

    static Vector2 One;

    Vector2() : Vector2(0) {
    }

    Vector2(float v) : Vector2(v, v) {
    }

    Vector2(float x, float y) : x(x), y(y) {
    }

    Vector2(const Vector3& vector) : Vector2(vector.x, vector.y) {
    }

    // TODO: Remove!
    Vector2(glm::vec2 vec) : Vector2(vec.x, vec.y) {
    }

    // Operators
    Vector2& operator+=(const Vector2& rhs) {
      this->x += rhs.x;
      this->y += rhs.y;
      return *this;
    }

    Vector2& operator-=(const Vector2& rhs) {
      this->x -= rhs.x;
      this->y -= rhs.y;
      return *this;
    }

    Vector2& operator*=(const Vector2& rhs) {
      this->x *= rhs.x;
      this->y *= rhs.y;
      return *this;
    }

    Vector2& operator*=(const float& val) {
      this->x *= val;
      this->y *= val;
      return *this;
    }

    Vector2& operator/=(const float& val) {
      this->x /= val;
      this->y /= val;
      return *this;
    }

    friend Vector2 operator+(Vector2 lhs, const Vector2& rhs) {
      lhs += rhs;
      return lhs;
    }

    friend Vector2 operator-(Vector2 lhs, const Vector2& rhs) {
      lhs -= rhs;
      return lhs;
    }

    friend Vector2 operator*(Vector2 lhs, const Vector2& rhs) {
      lhs *= rhs;
      return lhs;
    }

    friend Vector2 operator*(Vector2 lhs, const float& val) {
      lhs *= val;
      return lhs;
    }

    friend Vector2 operator/(Vector2 lhs, const float& val) {
      lhs /= val;
      return lhs;
    }

    static float dot(const Vector2& left, const Vector2& right) {
      Vector2 tmp = left * right;
      return tmp.x + tmp.y;
    }

    static float distance(const Vector2& left, const Vector2& right) {
      return left.distanceTo(right);
    }

    float distanceTo(const Vector2& vector) const {
      auto dist = sqrtf(
          powf(x - vector.x, 2) +
          powf(y - vector.y, 2));
      return dist;
    }

    Vector2 normalized() const {
      return *this / magnatude();
    }

    float magnatude() const {
      return Vector2::distance(Zero, *this);
    }

    // TODO: REMOVE THIS JUNK!
    glm::vec2 glm() const {
      return glm::vec2(x, y);
    }
  };

  typedef Vector3 Vector;
}

#endif //MATH_VECTOR_H