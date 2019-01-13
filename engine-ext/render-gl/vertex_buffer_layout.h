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
    
    VertexBufferLayout() : stride_(0) {}

    ~VertexBufferLayout() {}

    VertexBufferLayout* pushFloat(unsigned int count) { 
      elements_.push_back({ GL_FLOAT, count, GL_FALSE });
      stride_ += count * VertexBufferElement::sizeOf(GL_FLOAT);
      return this;
    } 

    VertexBufferLayout* pushUInt(unsigned int count) { 
      elements_.push_back({ GL_UNSIGNED_INT, count, GL_TRUE });
      stride_ += count * VertexBufferElement::sizeOf(GL_UNSIGNED_INT);
      return this;
    } 

    inline unsigned int stride() const {
      return stride_;
    }

    inline std::vector<VertexBufferElement> elements() const  {
      return elements_;
    }

  private:
    
    std::vector<VertexBufferElement> elements_;

    unsigned int stride_;

  };

}

#endif //RENDERER_VERTEX_BUFFER_LAYOUT_H