#include "render-vulkan/font_manager.h"

#include "render-vulkan/device.h"
#include "render-vulkan/font.h"

namespace wage::render {

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