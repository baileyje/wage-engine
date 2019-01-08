#ifndef RENDERER__MATERIAL_H
#define RENDERER__MATERIAL_H

#include <glad/glad.h>
#include "glm/ext.hpp"
#include <string>
#include <unordered_map>

#include "render-gl/program.h"
#include "render-gl/util.h"

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

    inline void setProgram(GlProgram* program) {
      this->program = program;
    }

    inline GlProgram* getProgram() {
      return program;
    }

    inline void setFloat(std::string name, float value) {
      GL_FAIL_CHECK(glUniform1f(glGetUniformLocation(program->id(), name.c_str()), value));
    }

    inline void setBool(std::string name, bool value) {
      GL_FAIL_CHECK(glUniform1i(glGetUniformLocation(program->id(), name.c_str()), (int)value));
    }

    inline void setInt(std::string name, int value) {
      GL_FAIL_CHECK(glUniform1i(glGetUniformLocation(program->id(), name.c_str()), value));
    }  

    inline void setMat4(std::string name,const glm::mat4 &value) {
      GL_FAIL_CHECK(glUniformMatrix4fv(glGetUniformLocation(program->id(), name.c_str()), 1, GL_FALSE, &value[0][0]));
    }

    inline void  setVec3(std::string name, const glm::vec3 &value) {
      GL_FAIL_CHECK(glUniform3fv(glGetUniformLocation(program->id(), name.c_str()), 1, &value[0]));
    }

    inline void setVec4(std::string name, const glm::vec4 &value) {
      GL_FAIL_CHECK(glUniform4fv(glGetUniformLocation(program->id(), name.c_str()), 1, &value[0]));
    }

    void bind() const;

    void unbind() const;

  private:

    GlProgram* program;

    UniformMap uniforms;

  };

}

#endif //RENDERER__MATERIAL_H
