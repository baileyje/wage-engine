#include "math/matrix.h"

namespace wage {

  Matrix Matrix::orthographic(float left, float right, float bottom, float top) {
    return Matrix(glm::ortho(left, right, bottom, top));
  }

  Matrix Matrix::orthographic(float left, float right, float top, float bottom, float zNear, float zFar) {
    return Matrix(glm::ortho(left, right, bottom, top, zNear, zFar));
  }

  Matrix Matrix::perspective(float fov, float aspect, float zNear, float zFar) {
    return Matrix(glm::perspective(fov, aspect, zNear, zFar));
  }

  Matrix Matrix::lookAt(Vector eye, Vector center, Vector up) {
    return Matrix(glm::lookAt(eye.glm(), center.glm(), up.glm()));
  }

  Matrix::Matrix() : Matrix(glm::mat4()) {}

  Matrix::Matrix(float val) : Matrix(glm::mat4(val)) {}

  Matrix Matrix::translate(Vector position) const {
    return Matrix(glm::translate(wrapped, position.glm()));
  }

  Matrix Matrix::scale(Vector scale) const {
    return Matrix(glm::scale(wrapped, scale.glm()));
  }

  Matrix Matrix::rotate(float angle, Vector vector) const {
    return Matrix(glm::rotate(wrapped, angle, vector.glm()));
  }

  Matrix& Matrix::operator*=(const Matrix& rhs) {
    wrapped *= rhs.wrapped;
    return *this;
  }

  Matrix::Matrix(Quaternion quat) : wrapped(glm::toMat4(quat.glm())) {
  }
}