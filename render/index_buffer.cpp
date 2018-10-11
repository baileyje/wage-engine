#include "render/index_buffer.h"

#include <glad/glad.h>

#include "render/util.h"

IndexBuffer::IndexBuffer(const unsigned int* data,  unsigned int count) : count(count) {
  FAIL_CHECK(glGenBuffers(1, &id));
  FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
  FAIL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  FAIL_CHECK(glDeleteBuffers(1, &id));
}

void IndexBuffer::bind() const {
  FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}

void IndexBuffer::unbind() const {
  FAIL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
