#ifndef RENDERER_FONT_MANAGER_H
#define RENDERER_FONT_MANAGER_H

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"
#include "render/font.h"

#include "render-gl/font.h"


namespace wage {

  class GlFontManager {
  
  public:

    GlFont* load(Font& font) {
      GlFont* glFont = cache[font.path()];
      if (glFont == nullptr) {
        glFont = make<GlFont>(font.path(), font.size());
        cache[font.path()] = glFont;
        _assetManager->load(glFont);
      }
      return glFont;
    }

    inline void assetManager(AssetManager* assetManager) {
      _assetManager = assetManager;
    }

  private:

    AssetManager* _assetManager;

    std::unordered_map<std::string, GlFont*> cache;

  };

}

#endif //RENDERER_FONT_MANAGER_H