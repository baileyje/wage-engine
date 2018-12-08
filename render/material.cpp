#include "render/material.h"

#include "glm/gtc/matrix_transform.hpp"

#include "render/util.h"

namespace wage {

  GlMaterial::GlMaterial(Shader* shader) : shader(shader) {
  }

  GlMaterial::~GlMaterial() {
  }

  void GlMaterial::bind() const {
    shader->bind();  
  }

  void GlMaterial::unbind() const {
    shader->unbind();
  }

}