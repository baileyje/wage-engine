#ifndef RENDERER__MATERIAL_H
#define RENDERER__MATERIAL_H

#include <glad/glad.h>
#include "glm/ext.hpp"
#include <string>
#include <unordered_map>

#include "render-gl/program.h"
#include "render-gl/util.h"

#include "math/matrix.h"

namespace wage {

  struct Uniform {

    unsigned int type;

    void* value;
    
  };

  typedef std::unordered_map<std::string, Uniform> UniformMap;

  class GlMaterial {

  public:

    GlMaterial(GlProgram* program);

    ~GlMaterial();

    inline void program(GlProgram* program) {
      _program = program;
    }

    inline GlProgram* program() {
      return _program;
    }

    inline void setFloat(std::string name, float value) {
      GL_FAIL_CHECK(glUniform1f(glGetUniformLocation(_program->id(), name.c_str()), value));
    }

    inline void setBool(std::string name, bool value) {
      GL_FAIL_CHECK(glUniform1i(glGetUniformLocation(_program->id(), name.c_str()), (int)value));
    }

    inline void setInt(std::string name, int value) {
      GL_FAIL_CHECK(glUniform1i(glGetUniformLocation(_program->id(), name.c_str()), value));
    }  

    inline void setMat4(std::string name, const Matrix &value) {
      auto mat4 = value.glm();
      GL_FAIL_CHECK(glUniformMatrix4fv(glGetUniformLocation(_program->id(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4)));
    }

    inline void  setVec3(std::string name, const Vector &value) {
      // TODO: IS this real???
      auto v3 = value.glm();
      GL_FAIL_CHECK(glUniform3fv(glGetUniformLocation(_program->id(), name.c_str()), 1, glm::value_ptr(v3)));
    }

    inline void setVec4(std::string name, const glm::vec4 &value) {
      GL_FAIL_CHECK(glUniform4fv(glGetUniformLocation(_program->id(), name.c_str()), 1, &value[0]));
    }

    void bind() const;

    void unbind() const;

  private:

    GlProgram* _program;

    UniformMap uniforms;

  };

}

#endif //RENDERER__MATERIAL_H
