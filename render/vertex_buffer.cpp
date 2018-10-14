#include "render/vertex_buffer.h"

#include <glad/glad.h>
#include "render/util.h"

VertexBuffer::VertexBuffer(const void* data,  unsigned int size) {  
  FAIL_CHECK(glGenBuffers(1, &id));
  FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, id));
  FAIL_CHECK(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  FAIL_CHECK(glDeleteBuffers(1, &id));
}

void VertexBuffer::bind() const {  
  FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, id));
}

void VertexBuffer::unbind() const {
  FAIL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
