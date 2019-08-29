#ifndef RENDERER_VAO_MANAGER_H
#define RENDERER_VAO_MANAGER_H

#include <unordered_map>

#include "memory/allocator.h"
#include "render/mesh.h"

#include "render-gl/vertex_array.h"
#include "render-gl/index_buffer.h"

namespace wage {

  class VaoManager {

  public:
    VertexArray* load(Mesh* mesh) {
      VertexArray* vao = cache[mesh->id()];
      if (vao == nullptr) {
        vao = make<VertexArray>();
        vao->bind();
        // Create Verts Buff
        VertexBuffer* verts = make<VertexBuffer>(mesh->vertices().data(), mesh->vertices().size() * 3 * sizeof(float));
        verts->layout()->pushFloat(3);
        vao->addBuffer(verts);
        // Create Norms Buff
        VertexBuffer* norms = make<VertexBuffer>(mesh->normals().data(), mesh->normals().size() * 3 * sizeof(float));
        norms->layout()->pushFloat(3);
        vao->addBuffer(norms);
        // Create Texture Buff
        VertexBuffer* uvs = make<VertexBuffer>(mesh->uvs().data(), mesh->uvs().size() * 3 * sizeof(float));
        uvs->layout()->pushFloat(2);
        vao->addBuffer(uvs);

        IndexBuffer* indices = make<IndexBuffer>((const unsigned int*)mesh->indices().data(), mesh->indices().size());
        indices->bind();

        cache[mesh->id()] = vao;
      }
      return vao;
    }

  private:
    std::unordered_map<std::string, VertexArray*> cache;
  };
}

#endif //RENDERER_VAO_MANAGER_H