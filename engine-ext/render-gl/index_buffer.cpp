#include "render-gl/index_buffer.h"

#include <glad/glad.h>

#include "render-gl/util.h"

namespace wage {
  
  IndexBuffer::IndexBuffer(const unsigned int* data,  unsigned int count) : _count(count) {  
    GL_FAIL_CHECK(glGenBuffers(1, &id));
    GL_FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    GL_FAIL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
  }

  IndexBuffer::~IndexBuffer() {
      GL_FAIL_CHECK(glDeleteBuffers(1, &id));
  }

  void IndexBuffer::bind() const {
    GL_FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
  }

  void IndexBuffer::unbind() const {
      GL_FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  }

}