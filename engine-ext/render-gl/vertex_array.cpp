#include "render-gl/vertex_array.h"

#include "render-gl/util.h"

namespace wage {

  VertexArray::VertexArray() : index(0) {
    GL_FAIL_CHECK(glGenVertexArrays(1, &id));  
  }

  VertexArray::~VertexArray() {
    GL_FAIL_CHECK(glDeleteVertexArrays(1, &id));
  }

  void VertexArray::addBuffer(VertexBuffer* buffer) {
    buffer->bind();
    VertexBufferLayout* layout = buffer->layout();
    const auto& elements = layout->elements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
      auto& element = elements[i];
      GL_FAIL_CHECK(glEnableVertexAttribArray(index));
      GL_FAIL_CHECK(glVertexAttribPointer(index, element.count, element.type, element.normalized, layout->stride() , (void*)offset));
      offset += element.count * VertexBufferElement::sizeOf(element.type);
      index++;
    }  
  }

    void VertexArray::bind() const {
      GL_FAIL_CHECK(glBindVertexArray(id));
    }

    void VertexArray::unbind() const {
      GL_FAIL_CHECK(glBindVertexArray(0));
    }

}
