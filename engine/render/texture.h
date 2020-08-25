#pragma once

#include <string>

#include "assets/asset_spec.h"

namespace wage {
  namespace render {

    /**
     * Simple texture with basically no options other than providing an image path to load.
     */
    class Texture : public assets::AssetSpec {

    public:
      static Texture Default;

      Texture() : Texture("default.png") {
      }

      Texture(std::string key) : AssetSpec("texture", key) {
      }

      ~Texture() {}
    };
  }
}