#include "render-vulkan/text/font/manager.h"

#include "render-vulkan/core/device.h"
#include "render-vulkan/text/font/font.h"

namespace wage::render::vulkan {

  Font* FontManager::load(FontSpec fontSpec) {
    auto font = cache[fontSpec.key()];
    if (font) return font;
    font = assetManager->load<Font>(fontSpec);
    cache[fontSpec.key()] = font;
    return font;
  }

  void FontManager::destroy(Device* device) {
    for (std::pair<std::string, Font*> element : cache) {
      element.second->destroy(device);
    }
  }
}