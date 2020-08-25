#pragma once

#include <string>

#include "assets/asset_spec.h"

namespace wage {
  namespace audio {

    /**
     * Simple reference to and Audio clip.
     */
    class ClipSpec : public assets::AssetSpec {

    public:
      ClipSpec(std::string key) : AssetSpec("audio", key) {
      }

      ~ClipSpec() {}
    };
  }
}