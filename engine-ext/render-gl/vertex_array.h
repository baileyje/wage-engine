#pragma once

#include <glad/glad.h>
#include <vector>

#include "render-gl/vertex_buffer.h"
#include "render-gl/vertex_buffer_layout.h"

namespace wage {
  namespace render {

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

      std::vector<VertexBuffer*> buffers;
    };

  }
}
