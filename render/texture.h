#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "entity/component/texture.h"
#include "render/util.h"

class GlTexture {

public:

  GlTexture(Texture* texture) : texture(texture) {
  }

  ~GlTexture() {}

  inline void setTexture(Texture* texture) {
    this->texture = texture;
  }

  inline Texture* getTexture() {
    return texture;
  }

  void bind();

  inline void unbind() const {

  }

  static GlTexture Default;

private:

  Texture* texture;

};

#endif //RENDERER_TEXTURE_H
