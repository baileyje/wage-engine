#ifndef RENDERER_VERTEX_ARRAY_H
#define RENDERER_VERTEX_ARRAY_H

#include <glad/glad.h>

#include "render-gl/vertex_buffer.h"
#include "render-gl/vertex_buffer_layout.h"

namespace wage {

  class VertexArray {

  public:
    VertexArray();

    ~VertexArray();

    void addBuffer(VertexBuffer* buffer);

    void bind() const;

    void unbind() const;

  private:
    unsigned int id;

    unsigned int index;
  };
}

#endif //RENDERER_VERTEX_ARRAY_H