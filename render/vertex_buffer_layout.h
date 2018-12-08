#ifndef RENDERER_VERTEX_BUFFER_LAYOUT_H
#define RENDERER_VERTEX_BUFFER_LAYOUT_H

#include <glad/glad.h>
#include <vector>

namespace wage {

  struct VertexBufferElement {
    
    unsigned int type;

    unsigned int count;
    
    unsigned int normalized;

    static unsigned int getSizeOf(unsigned int type) {
      switch(type) {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
      }
      return 0;
    }
  };

  class VertexBufferLayout {

  public:
    
    VertexBufferLayout() : stride(0) {}

    ~VertexBufferLayout() {}

    VertexBufferLayout* pushFloat(unsigned int count) { 
      elements.push_back({ GL_FLOAT, count, GL_FALSE });
      stride += count * VertexBufferElement::getSizeOf(GL_FLOAT);
      return this;
    } 

    VertexBufferLayout* pushUInt(unsigned int count) { 
      elements.push_back({ GL_UNSIGNED_INT, count, GL_TRUE });
      stride += count * VertexBufferElement::getSizeOf(GL_UNSIGNED_INT);
      return this;
    } 

    inline unsigned int getStride() const {
      return stride;
    }

    inline std::vector<VertexBufferElement> getElements() const  {
      return elements;
    }

  private:
    
    std::vector<VertexBufferElement> elements;

    unsigned int stride;

  };

}

#endif //RENDERER_VERTEX_BUFFER_LAYOUT_H