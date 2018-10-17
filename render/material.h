#ifndef RENDERER__MATERIAL_H
#define RENDERER__MATERIAL_H

#include <glad/glad.h>
#include "glm/ext.hpp"
#include <string>
#include <unordered_map>

#include "render/shader/shader.h"

struct Uniform {
  
  unsigned int type;

  unsigned int count;

  void* value;
  
};

typedef std::unordered_map<std::string, Uniform> UniformMap;

class Material {

public:

  Material(Shader* shader);

  ~Material();

  inline void setShader(Shader* shader) {
    this->shader = shader;
  }

  inline Shader* getShader() {
    return shader;
  }

  inline void setFloat(std::string name, float* value, unsigned int count) {
    uniforms[name] = { GL_FLOAT, count, (void*)value };
  }

  inline void setMat4(std::string name, glm::mat4* value, unsigned int count) {
    uniforms[name] = { 999, count, (void*)value };
  }

  inline void  setVec3(std::string name, glm::vec3* value, unsigned int count) {
    uniforms[name] = { 1000, count, (void*)value };
  }

  void bind() const;

  void unbind() const;

private:

  Shader* shader;

  UniformMap uniforms;

};

#endif //RENDERER__MATERIAL_H
