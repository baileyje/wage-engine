#pragma once

#include <string>

#include "asset/spec.h"

namespace wage {
  namespace render {

    /**
     * Specification for a font which includes a font asset key and a size.
     */
    class FontSpec : public asset::AssetSpec {

    public:
      /**
       * Create a new font specification.
       */
      FontSpec(std::string key, int size = 48) : AssetSpec("font", key), _size(size) {}

      FontSpec() : FontSpec("*****INVALID*****", 0) {
      }

      /**
       * Get the size to render the font.
       */
      inline int size() {
        return _size;
      }

    private:
      int _size;
    };

  }
}