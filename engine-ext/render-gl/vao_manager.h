#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "render/components/mesh.h"
#include "render/mesh_manager.h"
#include "asset/manager.h"

#include "render-gl/vertex_array.h"
#include "render-gl/index_buffer.h"

namespace wage {

  namespace render {

    class VaoManager {

    public:
      VertexArray* load(MeshData* meshData) {
        if (!meshData->loaded()) {
          return nullptr;
        }
        auto vao = cache[meshData->spec().key()];
        // Only make a single cache item
        if (vao == nullptr) {
          vao = new VertexArray();
          cache[meshData->spec().key()] = vao;
          vao->bind();
          // Create Verts Buff
          auto verticies = new VertexBuffer(meshData->vertices().data(), static_cast<unsigned int>(meshData->vertices().size() * 3 * sizeof(float)));
          verticies->layout()->pushFloat(3);
          vao->addBuffer(verticies);
          // Create Norms Buff
          auto norms = new VertexBuffer(meshData->normals().data(), static_cast<unsigned int>(meshData->normals().size() * 3 * sizeof(float)));
          norms->layout()->pushFloat(3);
          vao->addBuffer(norms);
          // Create Texture Buff31
          auto uvs = new VertexBuffer(meshData->uvs().data(), static_cast<unsigned int>(meshData->uvs().size() * 3 * sizeof(float)));
          uvs->layout()->pushFloat(2);
          vao->addBuffer(uvs);
          auto indices = new IndexBuffer((const unsigned int*)meshData->indices().data(), meshData->indices().size());
          indices->bind();
        }
        return vao;
      }

      inline void meshManager(MeshManager* meshManager) {
        _meshManager = meshManager;
      }

      inline void reset() {
        // TODO: Need a scheme to handle resets on the render thread.....
        
        // for (auto it : cache) {
        //   delete it.second;
        // }
        cache.clear();
      }

    private:
      MeshManager* _meshManager;

      std::unordered_map<std::string, VertexArray*> cache;
    };
  }
}
