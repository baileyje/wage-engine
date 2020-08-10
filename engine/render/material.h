#pragma once

#include "math/color.h"
#include "render/texture.h"

#define MaterialComponent 31

namespace wage { namespace render {
  // TODO:  De-jank this
  class Material {

  public:
    Material() : Material(math::Color(1, 0, 1, 1)) {
    }

    Material(math::Color color) : _color(color) {
    }

    Material(Texture texture) : _color(math::Color(1, 0, 1, 1)), _texture(texture) {
    }

    ~Material() {}

    inline void color(math::Color color) {
      _color = color;
    }

    inline math::Color color() {
      return _color;
    }

    inline Texture texture() {
      return _texture;
    }

  private:
    math::Color _color;

    Texture _texture;
  };

} }