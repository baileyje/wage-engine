#pragma once

#include <string>

#include "assets/asset_spec.h"

#define MeshComponent 30

namespace wage {
  namespace render {

    enum class MeshType {
      cube,
      sphere,
      quad,
      custom
    };

    class Mesh : public assets::AssetSpec {

    public:
      Mesh() : Mesh("*****INVALID*****", MeshType::cube) {}

      Mesh(std::string key, MeshType type) : assets::AssetSpec("mesh", key), _meshType(type) {
      }

      Mesh(Mesh* templateMesh) : assets::AssetSpec("mesh", templateMesh->key()), _meshType(templateMesh->meshType()) {}

      virtual ~Mesh() {
      }

      inline MeshType meshType() {
        return _meshType;
      }

      static Mesh* Cube;

      static Mesh* Sphere;

      static Mesh* Quad;

    protected:
      MeshType _meshType = MeshType::cube;
    };
  }
}