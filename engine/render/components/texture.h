#pragma once

#include <string>

#include "assets/asset_spec.h"

namespace wage {
  namespace render {

    /**
     * Specification for a texture with basically no options other than providing an asset key to load.
     */
    class TextureSpec : public assets::AssetSpec {

    public:
      /**
       * Create a default texture.
       */
      TextureSpec() : TextureSpec("default.png") {
      }

      /**
       * Create a texture with a specific asset key.
       */
      TextureSpec(std::string key) : AssetSpec("texture", key) {
      }

      ~TextureSpec() {}
    };
  }
}