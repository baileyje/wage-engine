#ifndef MATH_MATRIX_H
#define MATH_MATRIX_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "math/vector.h"
#include "math/quaternion.h"


namespace wage {


  class Matrix {
  
  public:
  
    static Matrix orthographic(float left, float right, float top, float bottom);

    static Matrix orthographic(float left, float right, float top, float bottom, float zNear, float zFar);

    static Matrix perspective(float fov, float aspect, float zNear, float zFar);

    static Matrix lookAt(Vector eye, Vector center, Vector up);

    static Matrix translate(Matrix matrix, Vector position);

    static Matrix scale(Matrix matrix, Vector scale);


    // Matrix translation = glm::translate(glm::mat4(1), _position.glm());
    //   Matrix scale = glm::scale(glm::mat4(1), _scale.glm());

    Matrix();

    Matrix(float val);

    Matrix(Quaternion quat);

    Matrix translate(Vector position) const;

    Matrix scale(Vector scale) const;

    Matrix& operator*=(const Matrix& rhs);

    friend Matrix operator*(Matrix lhs, const Matrix& rhs) {
      lhs *= rhs;
      return lhs;
    }

    inline glm::mat4 glm() const {
      return wrapped;
    }

  private:

    Matrix(glm::mat4 wrapped) : wrapped(wrapped) {}

    glm::mat4 wrapped;

  };

}

#endif //MATH_MATRIX_H