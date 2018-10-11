#ifndef RENDERER_VERTEX_BUFFER_H
#define RENDERER_VERTEX_BUFFER_H

class VertexBuffer {

public:
  
  VertexBuffer(const void* data,  unsigned int size);

  ~VertexBuffer();

  void bind() const;

  void unbind() const;

private:
  
  unsigned int id;

};


#endif //RENDERER_VERTEX_BUFFER_H