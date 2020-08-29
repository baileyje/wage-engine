#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"

#include "render/components/mesh.h"
#include "render/mesh_data.h"

namespace wage {
  namespace render {

    /**
     * Manager responsible for loading mesh data for provided specs. This is mostly a wrapper over the asset manager,
     * but provides a means to return primitive meshes likes cubes.
     */
    class MeshManager {

    public:
      /**
       * Load the mesh data for the provided mesh spec. This will return a primitive mesh if the spec key mataches
       * that of a pre-generated mesh/
       */
      MeshData* load(MeshSpec mesh) {
        auto primitive = primitives[mesh.key()];
        if (primitive != nullptr) {
          return primitive;
        }
        return _assetManager->load<MeshData>(mesh);
      }

      /**
       * Set the asset manager to use for loading mesh data.
       */
      inline void assetManager(assets::Manager* assetManager) {
        _assetManager = assetManager;
      }

      /**
       * Generate the primitive mesh data.
       */
      void generatePrimitives();

    private:
      assets::Manager* _assetManager;

      std::unordered_map<std::string, MeshData*> primitives;
    };

  }
}
