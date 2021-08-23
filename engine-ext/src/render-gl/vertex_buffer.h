#pragma once

#include "render-gl/vertex_buffer_layout.h"

namespace wage { namespace render {

  class VertexBuffer {

  public:
    VertexBuffer(const void* data, const unsigned int size);

    ~VertexBuffer();

    void bind() const;

    void unbind() const;

    VertexBufferLayout* layout();

  private:
    unsigned int id;

    VertexBufferLayout _layout;
  };

} }
