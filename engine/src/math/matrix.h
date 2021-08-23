#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "math/vector.h"
#include "math/quaternion.h"

namespace wage { namespace math {

  class Matrix {

  public:
    static Matrix orthographic(float left, float right, float top, float bottom);

    static Matrix orthographic(float left, float right, float top, float bottom, float zNear, float zFar);

    static Matrix perspective(float fov, float aspect, float zNear, float zFar);

    static Matrix lookAt(Vector eye, Vector center, Vector up);

    static Matrix translate(Matrix matrix, Vector position);

    static Matrix scale(Matrix matrix, Vector scale);

    Matrix();

    Matrix(float val);

    Matrix(Quaternion quat);

    Matrix translate(Vector position) const;

    Matrix scale(Vector scale) const;

    Matrix rotate(float angle, Vector vector) const;

    Matrix& operator*=(const Matrix& rhs);

    friend Matrix operator*(Matrix lhs, const Matrix& rhs) {
      lhs *= rhs;
      return lhs;
    }

    inline glm::mat4 glm() const {
      return wrapped;
    }

    void decompose(Vector3& position, Vector3& scale, Quaternion& rotation) {
      glm::vec3 tmpScale;
      glm::quat tmpRotation;
      glm::vec3 tmpTranslation;
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::decompose(wrapped, tmpScale, tmpRotation, tmpTranslation, skew, perspective);
      position.x = tmpTranslation.x;
      position.y = tmpTranslation.y;
      position.z = tmpTranslation.z;
      scale.x = tmpScale.x;
      scale.y = tmpScale.y;
      scale.z = tmpScale.z;
      rotation.x = tmpRotation.x;
      rotation.y = tmpRotation.y;
      rotation.z = tmpRotation.z;
      rotation.w = tmpRotation.w;
    }

  private:
    Matrix(glm::mat4 wrapped) : wrapped(wrapped) {}

    glm::mat4 wrapped;
  };

} }
