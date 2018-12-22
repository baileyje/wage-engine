#ifndef RENDERER_FONT_MANAGER_H
#define RENDERER_FONT_MANAGER_H

#include <unordered_map>

#include "assets/manager.h"

#include "render-gl/font.h"

#include "render/font.h"

namespace wage {

  class GlFontManager {
  
  public:

    GlFont* load(Font& font) {
      GlFont* glFont = cache[font.path()];
      if (glFont == nullptr) {
        glFont = new GlFont(font.path(), font.size());
        cache[font.path()] = glFont;
        assetManager->load(glFont);
      }
      return glFont;
    }

    inline void setAssetManager(AssetManager* assetManager) {
      this->assetManager = assetManager;
    }

  private:

    AssetManager* assetManager;

    std::unordered_map<std::string, GlFont*> cache;

  };

}

#endif //RENDERER_FONT_MANAGER_H