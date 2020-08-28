#pragma once

#include "component/color.h"
#include "render/components/texture.h"

#define MaterialComponent 31

namespace wage {
  namespace render {

    /**
     * Specification for a render material that currently supports only a color and or texture.
     */
    class MaterialSpec {

    public:
      /**
       * Create a default material.
       */
      MaterialSpec() : MaterialSpec(component::Color(1, 0, 1, 1)) {
      }

      /**
       * Create a material with a specific color to render.
       */
      MaterialSpec(component::Color color) : _color(color) {
      }

      /**
       * Create a material with a specific texture to render.
       */
      MaterialSpec(TextureSpec texture) : _color(component::Color(1, 0, 1, 1)), _texture(texture) {
      }

      ~MaterialSpec() {}

      /**
       * Set the color to render for this material.
       */
      inline void color(component::Color color) {
        _color = color;
      }

      /**
       * Get the color to render for this material.
       */
      inline component::Color color() {
        return _color;
      }

      /**
       * Set the texture to render for this material.
       */
      inline void texture(TextureSpec texture) {
        _texture = texture;
      }

      /**
       * Get the texture to render for this material.
       */
      inline TextureSpec texture() {
        return _texture;
      }

    private:
      component::Color _color;

      TextureSpec _texture;
    };

  }
}