#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "entity/component/render/texture.h"
#include "render/util.h"

#include "fs/file_system.h"

namespace wage {

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

    void unbind();

    static GlTexture* Default;

  private:

    unsigned int id;

    bool loaded;

    bool pushed;

    Texture* texture;

    int width;

    int height;

    int channels;

    unsigned char *data;

  };

}

#endif //RENDERER_TEXTURE_H
