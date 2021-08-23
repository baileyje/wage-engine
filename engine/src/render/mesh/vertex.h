#pragma once

#include <vector>

#include "math/vector.h"

namespace wage {
  namespace render {

    struct Vertex {
      math::Vector3 position;
      math::Vector3 color;
      math::Vector3 normal;
      math::Vector2 textureCoord;

      bool operator==(const Vertex& other) const {
        return position == other.position && color == other.color && normal == other.normal && textureCoord == other.textureCoord;
      }
    };
  }
}

namespace std {
  template <>
  struct hash<wage::render::Vertex> {
    size_t operator()(wage::render::Vertex const& vertex) const {
      return ((hash<wage::math::Vector3>()(vertex.position) ^ (hash<wage::math::Vector3>()(vertex.textureCoord) << 1)) >> 1) ^ (hash<wage::math::Vector3>()(vertex.normal) << 1);
    }
  };
}