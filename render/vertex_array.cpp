#include "render/vertex_array.h"

#include "render/util.h"

namespace wage {

  VertexArray::VertexArray() : index(0) {
    GL_FAIL_CHECK(glGenVertexArrays(1, &id));  
  }

  VertexArray::~VertexArray() {
    GL_FAIL_CHECK(glDeleteVertexArrays(1, &id));
  }

  void VertexArray::addBuffer(VertexBuffer* buffer) {
    buffer->bind();
    VertexBufferLayout* layout = buffer->getLayout();
    const auto& elements = layout->getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
      auto& element = elements[i];
      GL_FAIL_CHECK(glEnableVertexAttribArray(index));
      GL_FAIL_CHECK(glVertexAttribPointer(index, element.count, element.type, element.normalized, layout->getStride() , (void*)offset));
      offset += element.count * VertexBufferElement::getSizeOf(element.type);
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
