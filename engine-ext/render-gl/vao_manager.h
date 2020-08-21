#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "render/mesh.h"
#include "render/mesh_manager.h"
#include "assets/manager.h"

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
          vao = memory::make<VertexArray>();
          cache[meshData->spec().key()] = vao;
          vao->bind();
          // Create Verts Buff
          auto verts = memory::make<VertexBuffer>(meshData->vertices().data(), meshData->vertices().size() * 3 * sizeof(float));
          verts->layout()->pushFloat(3);
          vao->addBuffer(verts);
          // Create Norms Buff
          auto norms = memory::make<VertexBuffer>(meshData->normals().data(), meshData->normals().size() * 3 * sizeof(float));
          norms->layout()->pushFloat(3);
          vao->addBuffer(norms);
          // Create Texture Buff
          auto uvs = memory::make<VertexBuffer>(meshData->uvs().data(), meshData->uvs().size() * 3 * sizeof(float));
          uvs->layout()->pushFloat(2);
          vao->addBuffer(uvs);
          auto indices = memory::make<IndexBuffer>((const unsigned int*)meshData->indices().data(), meshData->indices().size());
          indices->bind();
        }
        return vao;
      }

      inline void meshManager(MeshManager* meshManager) {
        _meshManager = meshManager;
      }

    private:
      MeshManager* _meshManager;

      std::unordered_map<std::string, VertexArray*> cache;
    };
  }
}
