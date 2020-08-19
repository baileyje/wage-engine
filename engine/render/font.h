#pragma once

#include <string>

#include "assets/asset_spec.h"

namespace wage {
  namespace render {

    class Font : public assets::AssetSpec {

    public:
      Font(std::string key, int size = 48) : AssetSpec("font", key), _size(size) {}

      Font() : Font("*****INVALID*****", 0) {
      }

      inline int size() {
        return _size;
      }

    private:
      int _size;
    };

  }
}