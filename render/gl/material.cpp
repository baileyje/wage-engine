#include "render/gl/material.h"

#include "glm/gtc/matrix_transform.hpp"

#include "render/gl/util.h"

namespace wage {

  GlMaterial::GlMaterial(GlShader* shader) : shader(shader) {
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