#ifndef RENDERER_TEXTURE_MANAGER_H
#define RENDERER_TEXTURE_MANAGER_H

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"

#include "render-gl/texture.h"

namespace wage {

  class GlTextureManager {
  
  public:

    GlTexture* load(Texture* texture) {
      GlTexture* glTexture = cache[texture->id()];
      if (glTexture == nullptr) {
        glTexture = make<GlTexture>(texture);
        cache[texture->id()] = glTexture;
        assetManager->load(glTexture);
      }
      return glTexture;
    }

    inline void setAssetManager(AssetManager* assetManager) {
      this->assetManager = assetManager;
    }

  private:

    AssetManager* assetManager;

    std::unordered_map<std::string, GlTexture*> cache;

  };

}

#endif //RENDERER_TEXTURE_MANAGER_H