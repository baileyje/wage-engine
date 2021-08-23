#pragma once

#include "render/components/texture.h"
#include "asset/manager.h"

#include "render-gl/util.h"

namespace wage {
  namespace render {

    class GlTexture : public asset::Asset {

    public:
      GlTexture(TextureSpec texture) : Asset(texture), pushed(false), _texture(texture) {
      }

      ~GlTexture();

      inline TextureSpec texture() const {
        return _texture;
      }

      void load(asset::Manager* assetManager);

      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);

      void bind();

      void unbind();

    private:
      void push();

      unsigned int id;

      bool pushed;

      TextureSpec _texture;

      std::unique_ptr<Asset> vertexSource;

      int width;

      int height;

      int channels;

      unsigned char* data;
    };

  }
}
