#ifndef RENDERER_TEXTURE_H
#define RENDERER_TEXTURE_H

#include "render/texture.h"
#include "assets/manager.h"

#include "render-gl/util.h"

namespace wage {

  class GlTexture : public Asset {

  public:

    GlTexture(Texture* texture) : Asset(texture->path()), pushed(false), texture_(texture) {  
    }

    ~GlTexture();

    inline Texture* texture() {
      return texture_;
    }

    void load(AssetManager* assetManager);

    void onLoad(Buffer* buffer);

    void bind();

    void unbind();

  private:

    void push();
    
    unsigned int id;

    bool pushed;

    Texture* texture_;

    std::unique_ptr<Asset> vertexSource;

    int width;

    int height;

    int channels;

    unsigned char *data;

  };

}

#endif //RENDERER_TEXTURE_H
