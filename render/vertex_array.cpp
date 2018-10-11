#include "render/vertex_array.h"

#include "render/util.h"

VertexArray::VertexArray() {
  FAIL_CHECK(glGenVertexArrays(1, &id));  
}

VertexArray::~VertexArray() {
  FAIL_CHECK(glDeleteVertexArrays(1, &id));
}

void VertexArray::addBuffer(const VertexBuffer& buffer, const VertexBufferLayout &layout) {
  buffer.bind();
  const auto& elements = layout.getElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    auto& element = elements[i];
    FAIL_CHECK(glEnableVertexAttribArray(i));
    FAIL_CHECK(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride() , (void*)offset));
    offset += element.count * VertexBufferElement::getSizeOf(element.type);
  }  
}

  void VertexArray::bind() const {
    FAIL_CHECK(glBindVertexArray(id));
  }

  void VertexArray::unbind() const {
    FAIL_CHECK(glBindVertexArray(id));
  }


