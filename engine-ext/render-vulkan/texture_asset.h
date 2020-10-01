#pragma once

#include <stdexcept>

#include "asset/asset.h"
#include "render/components/texture.h"
#include "render-vulkan/texture.h"

namespace wage {
  namespace render {
    
    
    class TextureAsset : public asset::Asset {
    public:
    
      TextureAsset(TextureSpec texture);

      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);

      Texture texture;
    };
  }
}