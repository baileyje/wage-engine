#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"

#include "render-gl/texture.h"

namespace wage { namespace render {

  class GlTextureManager {

  public:
    GlTexture* load(Texture texture) {
      GlTexture* glTexture = cache[texture.id()];
      if (glTexture == nullptr) {
        glTexture = memory::make<GlTexture>(texture);
        cache[texture.id()] = glTexture;
        _assetManager->load(glTexture);
      }
      return glTexture;
    }

    inline void assetManager(assets::Manager* assetManager) {
      _assetManager = assetManager;
    }

  private:
    assets::Manager* _assetManager;

    std::unordered_map<std::string, GlTexture*> cache;
  };

} }
