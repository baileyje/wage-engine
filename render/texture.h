#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "entity/component/texture.h"
#include "render/util.h"

#include "fs/file_system.h"

class GlTexture {

public:

  GlTexture(Texture* texture);

  ~GlTexture();

  inline void setTexture(Texture* texture) {
    this->texture = texture;
  }

  inline Texture* getTexture() {
    return texture;
  }

  void load(FileSystem* fileSystem);

  void bind();

  inline void unbind() const {

  }

  static GlTexture Default;

private:

  unsigned int id;

  bool loaded;

  Texture* texture;

  int width;

  int height;

  int channels;

  unsigned char *data;

};

#endif //RENDERER_TEXTURE_H
