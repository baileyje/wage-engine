#ifndef RENDERER_INDEX_BUFFER_H
#define RENDERER_INDEX_BUFFER_H

namespace wage {

  class IndexBuffer {

  public:
    IndexBuffer(const unsigned int* data, unsigned int count);

    ~IndexBuffer();

    void bind() const;

    void unbind() const;

    inline unsigned int count() const {
      return _count;
    }

  private:
    // const unsigned int* data;

    unsigned int _count;

    unsigned int id;
  };
}

#endif //RENDERER_INDEX_BUFFER_H