#pragma once

#include <string>

#include "asset/spec.h"

#define MeshComponent 30

namespace wage {
  namespace render {

    /**
     * Specification for a mesh to be rendered that includes an asset key use to load the mesh data.
     */
    class MeshSpec : public asset::AssetSpec {

    public:
      /**
       * Primitive cube mesh key.
       */
      static asset::AssetSpec::Key Cube;

      /**
       * Primitive sphere mesh key.
       */
      static asset::AssetSpec::Key Sphere;

      /**
       * Primitive quad mesh key.
       */
      static asset::AssetSpec::Key Quad;

      /**
       * Default mesh spec. Defaults to Cube.
       */
      MeshSpec() : MeshSpec(Cube) {}

      /**
       * Create a mesh spec with an asset key pointing to the mesh object data.
       */
      MeshSpec(std::string key) : asset::AssetSpec("mesh", key) {}

      virtual ~MeshSpec() {
      }
    };
  }
}