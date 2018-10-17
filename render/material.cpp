#include "render/material.h"

#include "glm/gtc/matrix_transform.hpp"

#include "render/util.h"


Material::Material(Shader* shader) : shader(shader) {
}

Material::~Material() {
}

void Material::bind() const {
  shader->bind();
  for (auto uni : uniforms) {
    switch(uni.second.type) {
      case 999: {
        GLuint matId;
        FAIL_CHECK(matId = glGetUniformLocation(shader->getId(), uni.first.c_str())); 
        glm::mat4* mat = (glm::mat4*)uni.second.value;
        FAIL_CHECK(glUniformMatrix4fv(matId, 1, GL_FALSE, glm::value_ptr(*mat)));
        break;
      }
      case 1000:
        GLuint vec3Id;
        FAIL_CHECK(vec3Id = glGetUniformLocation(shader->getId(), uni.first.c_str())); 
        glm::vec3* vec3 = (glm::vec3*)uni.second.value;
        FAIL_CHECK(glUniform3fv(vec3Id, 1, glm::value_ptr(*vec3)));
        break;
    }
  }
}

void Material::unbind() const {
  shader->unbind();
}