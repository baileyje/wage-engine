#include "engine-ext/render-gl/vertex_buffer.h"

#include <glad/glad.h>

#include "engine-ext/render-gl/util.h"

namespace wage {
  VertexBuffer::VertexBuffer(const void* data,  unsigned int size) {  
    GL_FAIL_CHECK(glGenBuffers(1, &id));
    GL_FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, id));
    GL_FAIL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }

  VertexBuffer::~VertexBuffer() {
    GL_FAIL_CHECK(glDeleteBuffers(1, &id));
  }

  void VertexBuffer::bind() const {  
    GL_FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, id));
  }

  void VertexBuffer::unbind() const {
    GL_FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
  }

  VertexBufferLayout* VertexBuffer::getLayout() {
    return &layout;
  }

}