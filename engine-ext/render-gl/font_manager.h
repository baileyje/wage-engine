#pragma once

#include <unordered_map>

#include "memory/allocator.h"
#include "assets/manager.h"
#include "render/font.h"

#include "render-gl/font.h"

namespace wage { namespace render {

  class GlFontManager {

  public:
    GlFont* load(Font& font) {
      GlFont* glFont = cache[font.path()];
      if (glFont == nullptr) {
        glFont = memory::make<GlFont>(font.path(), font.size());
        cache[font.path()] = glFont;
        _assetManager->load(glFont);
      }
      return glFont;
    }

    inline void assetManager(assets::Manager* assetManager) {
      _assetManager = assetManager;
    }

  private:
    assets::Manager* _assetManager;

    std::unordered_map<std::string, GlFont*> cache;
  };

} }