#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "engine/render/texture.h"
#include "engine/assets/manager.h"

#include "engine-ext/render-gl/util.h"

namespace wage {

  class GlTexture : public Asset {

  public:

    GlTexture(Texture* texture);

    ~GlTexture();

    inline Texture* getTexture() {
      return texture;
    }

    void load(AssetManager* assetManager);

    void onLoad(Buffer* buffer);

    void bind();

    void unbind();

  private:

    void push();
    
    unsigned int id;

    bool pushed;

    Texture* texture;

    std::unique_ptr<Asset> vertexSource;

    int width;

    int height;

    int channels;

    unsigned char *data;

  };

}

#endif //RENDERER_TEXTURE_H
