#ifndef RENDERER_MESH_H
#define RENDERER_MESH_H

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_buffer_layout.h"
#include "render/vertex_array.h"

class Mesh {

public:

  Mesh(const VertexBuffer* vbo, const IndexBuffer* ibo, const VertexBufferLayout* layout);
  
  virtual ~Mesh();

  virtual void bind(VertexArray* vao) const;
  
  virtual void unbind() const;

  inline unsigned int getElementCount() {
    return ibo->getCount();
  }

private:

  const VertexBuffer* vbo;

  const IndexBuffer* ibo;

  const VertexBufferLayout* layout;

};


#endif //RENDERER_MESH_H