#ifndef RENDERER_VERTEX_BUFFER_H
#define RENDERER_VERTEX_BUFFER_H

#include "render-gl/vertex_buffer_layout.h"

namespace wage {
  
  class VertexBuffer {

  public:
    
    VertexBuffer(const void* data,  const unsigned int size);

    ~VertexBuffer();

    void bind() const;

    void unbind() const;

    VertexBufferLayout* layout();

  private:

    unsigned int id;

    VertexBufferLayout _layout;
  };

}

#endif //RENDERER_VERTEX_BUFFER_H