#pragma once

#include "render/components/texture.h"
#include "asset/manager.h"
#include "render-vulkan/model/texture_asset.h"

namespace wage::render::vulkan {

  class VulkanTextureManager {
  public:
    void assetManager(asset::Manager* assetManager);

    TextureAsset* load(TextureSpec spec);

    void destroy(Device* device);

  private:
    std::unordered_map<std::string, TextureAsset*> cache;

    asset::Manager* _assetManager;
  };

}