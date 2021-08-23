#include "render-vulkan/model/texture_manager.h"

namespace wage::render::vulkan {

  void VulkanTextureManager::assetManager(asset::Manager* assetManager) {
    _assetManager = assetManager;
  }

  TextureAsset* VulkanTextureManager::load(TextureSpec spec) {
    auto key = spec.key();
    if (cache.find(key) != cache.end()) {
      return cache[key];
    }
    TextureAsset* texture = _assetManager->load<TextureAsset>(spec);
    cache[key] = texture;
    return texture;
  }

  void VulkanTextureManager::destroy(Device* device) {
    for (auto pair : cache) {
      pair.second->texture.destroy(device);
    }
  }
}