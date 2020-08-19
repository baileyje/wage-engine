#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"

#include "render/mesh.h"
#include "render/mesh_data.h"

namespace wage {
  namespace render {

    class MeshManager {

    public:
      MeshData* load(Mesh mesh) {
        auto primitive = primitives[mesh.key()];
        if (primitive != nullptr) {
          return primitive;
        }
        return _assetManager->load<MeshData>(mesh);
      }

      inline void assetManager(assets::Manager* assetManager) {
        _assetManager = assetManager;
      }

      void generatePrimitives();

    private:
      assets::Manager* _assetManager;

      std::unordered_map<std::string, MeshData*> primitives;
    };

  }
}
