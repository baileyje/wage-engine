#ifndef RENDERER_VAO_MANAGER_H
#define RENDERER_VAO_MANAGER_H

#include <unordered_map>

#include "render/mesh.h"
#include "render-gl/vertex_array.h"
#include "render-gl/index_buffer.h"

namespace wage {

  class VaoManager {
  
  public:

    VertexArray* load(Mesh* mesh) {
      VertexArray* vao = cache[mesh->getId()];
      if (vao == nullptr) {
        vao = new VertexArray();
        vao->bind();        
        // Create Verts Buff    
        VertexBuffer* verts = new VertexBuffer(mesh->getVertices().data(), mesh->getVertices().size() * 3 * sizeof(float));
        verts->getLayout()->pushFloat(3);
        vao->addBuffer(verts);
        // Create Norms Buff
        VertexBuffer* norms = new VertexBuffer(mesh->getNormals().data(), mesh->getNormals().size() * 3 * sizeof(float));
        norms->getLayout()->pushFloat(3);
        vao->addBuffer(norms);
        // Create Texture Buff
        VertexBuffer* uvs = new VertexBuffer(mesh->getUvs().data(), mesh->getUvs().size() * 3 * sizeof(float));
        uvs->getLayout()->pushFloat(2);
        vao->addBuffer(uvs);  
        
        IndexBuffer* indices = new IndexBuffer((const unsigned int*)mesh->getIndices().data(), mesh->getIndices().size());
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