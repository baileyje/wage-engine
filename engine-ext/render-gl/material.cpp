#include "render-gl/material.h"

#include "render-gl/util.h"

namespace wage {

  GlMaterial::GlMaterial(GlProgram* program) : _program(program) {
  }

  GlMaterial::~GlMaterial() {
  }

  void GlMaterial::bind() const {
    _program->bind();
  }

  void GlMaterial::unbind() const {
    _program->unbind();
  }
}