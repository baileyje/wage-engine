#ifndef RENDERER_VAO_MANAGER_H
#define RENDERER_VAO_MANAGER_H

#include <unordered_map>

#include "engine/memory/allocator.h"
#include "engine/render/mesh.h"

#include "engine-ext/render-gl/vertex_array.h"
#include "engine-ext/render-gl/index_buffer.h"

namespace wage {

  class VaoManager {
  
  public:

    VertexArray* load(ComponentReference<Mesh> mesh) {
      VertexArray* vao = cache[mesh->getId()];
      if (vao == nullptr) {
        vao = make<VertexArray>();
        vao->bind();        
        // Create Verts Buff    
        VertexBuffer* verts = make<VertexBuffer>(mesh->getVertices().data(), mesh->getVertices().size() * 3 * sizeof(float));
        verts->getLayout()->pushFloat(3);
        vao->addBuffer(verts);
        // Create Norms Buff
        VertexBuffer* norms = make<VertexBuffer>(mesh->getNormals().data(), mesh->getNormals().size() * 3 * sizeof(float));
        norms->getLayout()->pushFloat(3);
        vao->addBuffer(norms);
        // Create Texture Buff
        VertexBuffer* uvs = make<VertexBuffer>(mesh->getUvs().data(), mesh->getUvs().size() * 3 * sizeof(float));
        uvs->getLayout()->pushFloat(2);
        vao->addBuffer(uvs);  
        
        IndexBuffer* indices = make<IndexBuffer>((const unsigned int*)mesh->getIndices().data(), mesh->getIndices().size());
        indices->bind();
        
        cache[mesh->getId()] = vao;
      }
      return vao;
    }

  private:

    std::unordered_map<std::string, VertexArray*> cache;

  };

}

#endif //RENDERER_VAO_MANAGER_H