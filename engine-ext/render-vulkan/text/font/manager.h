#pragma once

#include <unordered_map>
#include "render/components/font.h"
#include "asset/manager.h"

namespace wage::render::vulkan {

  class Font;
  class Device;

  class FontManager {
  public:
    Font* load(FontSpec fontSpec);

    void destroy(Device* device);

    asset::Manager* assetManager;

  private:
    std::unordered_map<std::string, Font*> cache;
  };
}