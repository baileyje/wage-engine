#include "render-gl/material.h"

#include "glm/gtc/matrix_transform.hpp"

#include "render-gl/util.h"

namespace wage {

  GlMaterial::GlMaterial(GlProgram* program) : program_(program) {
  }

  GlMaterial::~GlMaterial() {
  }

  void GlMaterial::bind() const {
    program_->bind();  
  }

  void GlMaterial::unbind() const {
    program_->unbind();
  }

}