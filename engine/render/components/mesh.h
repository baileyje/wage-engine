#pragma once

#include <string>

#include "math/math.h"
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
       * The mesh's local transform with respect to the entity transform.
       */
      math::Transform transform;

      /**
       * Default mesh spec. Defaults to Cube.
       */
      MeshSpec();

      /**
       * Create a mesh spec with an asset key pointing to the mesh object data.
       */
      MeshSpec(std::string key);

      ~MeshSpec();
    };
  }
}