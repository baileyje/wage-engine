#ifndef RENDERER_VERTEX_BUFFER_LAYOUT_H
#define RENDERER_VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>

namespace wage {

  struct VertexBufferElement {

    unsigned int type;

    unsigned int count;

    unsigned int normalized;

    static unsigned int sizeOf(unsigned int type) {
      switch (type) {
      case GL_FLOAT:
        return 4;
      case GL_UNSIGNED_INT:
        return 4;
      case GL_UNSIGNED_BYTE:
        return 1;
      }
      return 0;
    }
  };

  class VertexBufferLayout {

  public:
    VertexBufferLayout() : _stride(0) {}

    ~VertexBufferLayout() {}

    VertexBufferLayout* pushFloat(unsigned int count) {
      _elements.push_back({GL_FLOAT, count, GL_FALSE});
      _stride += count * VertexBufferElement::sizeOf(GL_FLOAT);
      return this;
    }

    VertexBufferLayout* pushUInt(unsigned int count) {
      _elements.push_back({GL_UNSIGNED_INT, count, GL_TRUE});
      _stride += count * VertexBufferElement::sizeOf(GL_UNSIGNED_INT);
      return this;
    }

    inline unsigned int stride() const {
      return _stride;
    }

    inline std::vector<VertexBufferElement> elements() const {
      return _elements;
    }

  private:
    std::vector<VertexBufferElement> _elements;

    unsigned int _stride;
  };
}

#endif //RENDERER_VERTEX_BUFFER_LAYOUT_H