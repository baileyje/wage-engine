#pragma once

#include "render/texture.h"
#include "assets/manager.h"

#include "render-gl/util.h"

namespace wage {
  namespace render {

    class GlTexture : public assets::Asset {

    public:
      GlTexture(Texture texture) : Asset(texture), pushed(false), _texture(texture) {
      }

      ~GlTexture();

      inline Texture texture() const {
        return _texture;
      }

      void load(assets::Manager* assetManager);

      void onLoad(memory::Buffer buffer);

      void bind();

      void unbind();

    private:
      void push();

      unsigned int id;

      bool pushed;

      Texture _texture;

      std::unique_ptr<Asset> vertexSource;

      int width;

      int height;

      int channels;

      unsigned char* data;
    };

  }
}
