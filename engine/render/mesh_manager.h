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
      MeshData* load(Mesh* mesh) {
        auto data = _loaded[mesh->key()];
        if (data == nullptr) {
          data = memory::make<MeshData>(mesh->key());
          _loaded[mesh->key()] = data;
          _assetManager->load(data);
        }
        return data;
      }

      inline void assetManager(assets::Manager* assetManager) {
        _assetManager = assetManager;
      }

      void generatePrimitives();

    private:
      assets::Manager* _assetManager;

      std::unordered_map<std::string, MeshData*> _loaded;
    };

  }
}
