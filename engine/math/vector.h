#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "glm/ext.hpp"


namespace wage {

  // TODO: MAKE THIS WORK. Not working due to issue with virtual method in template class.  
  /*
  template <int Size>
  class VectorBase {
  
  public:

    static VectorBase<Size> Zero;

    // Operators
    VectorBase<Size>& operator+=(const VectorBase<Size>& rhs) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] += rhs[idx];
      }
      return *this;
    }

    VectorBase<Size>& operator-=(const VectorBase<Size>& rhs) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] -= rhs[idx];
      }
      return *this;
    }

    VectorBase<Size>& operator*=(const VectorBase<Size>& rhs) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] *= rhs[idx];
      }
      return *this;
    }

    VectorBase<Size>& operator*=(const float& val) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] *= val;
      }
      return *this;
    }

    VectorBase<Size>& operator/=(const VectorBase<Size>& rhs) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] /= rhs[idx];
      }
      return *this;
    }

    VectorBase<Size>& operator/=(const float& val) {
      for (int idx = 0; idx < Size; ++idx) {
        this[idx] /= val;
      }
      return *this;
    }

    virtual const float& operator[](int idx) const = 0;

    virtual float& operator[](int idx) = 0;
    
    friend VectorBase<Size> operator+(VectorBase<Size> lhs, const VectorBase<Size>& rhs) {
      lhs += rhs;
      return lhs;
    }

    friend VectorBase<Size> operator-(VectorBase<Size> lhs, const VectorBase<Size>& rhs) {
      lhs -= rhs;
      return lhs;
    }

    friend VectorBase<Size> operator*(VectorBase<Size> lhs, const VectorBase<Size>& rhs) {
      lhs *= rhs;
      return lhs;
    }

    friend VectorBase<Size> operator*(VectorBase<Size> lhs, const float& val) {
      lhs *= val;
      return lhs;
    }

    friend VectorBase<Size> operator/(VectorBase<Size> lhs, const float& val) {
      lhs /= val;
      return lhs;
    }

    static float dot(const VectorBase<Size>& left, const VectorBase<Size>& right) {
      VectorBase<Size> tmp = left * right;
      float sum = 0;
      for (int idx = 0; idx < Size; ++idx) {
        sum += tmp[idx];
      }
      return sum;
    }
    
    static float distance(const VectorBase<Size>& left, const VectorBase<Size>& right) {
      float sum = 0;
      for (int idx = 0; idx < Size; ++idx) {
        sum += powf(left[idx] - right[idx], 2);
      }
      return sqrtf(sum);
    }

    static VectorBase<Size> normalized(const VectorBase<Size>& vector) {
      return vector / vector.magnatude();
    }

    VectorBase<Size> normalized() const {
      return VectorBase<Size>::normalized(*this);
    }

    float magnatude() const {
      return VectorBase<Size>::distance(Zero, *this);
    }

  };
*/
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

    Vector3(float x, float y , float z) : x(x), y(y), z(z) {
    }

    // TODO: Remove!
    Vector3(glm::vec3 vec) : Vector3(vec.x, vec.y, vec.z) {
    }

    // // Operators
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
    
    static float distance(const Vector3& left, const Vector3& right) {
      auto dist = sqrtf(
        powf(left.x - right.x, 2) +
        powf(left.y - right.y, 2) +
        powf(left.z - right.z, 2) 
      );
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

    Vector2() : Vector2(0) {
    }

    Vector2(float v) : Vector2(v, v) {
    }

    Vector2(float x, float y) : x(x), y(y) {
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
        powf(y - vector.y, 2)
      );
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

  // typedef glm::vec2 Vector2;
  
  typedef Vector3 Vector;

}

#endif //MATH_VECTOR_H