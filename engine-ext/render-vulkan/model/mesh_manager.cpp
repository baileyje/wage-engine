#include "render-vulkan/model/mesh_manager.h"

namespace wage::render::vulkan {

  void VulkanMeshManager::assetManager(asset::Manager* assetManager) {
    meshManager.assetManager(assetManager);
    meshManager.generatePrimitives();
  }

  VulkanMesh* VulkanMeshManager::load(MeshSpec spec) {
    auto key = spec.key();
    if (cache.find(key) != cache.end()) {
      return cache[key];
    }
    VulkanMesh* mesh = new VulkanMesh(meshManager.load(spec));
    cache[key] = mesh;
    return mesh;
  }

  void VulkanMeshManager::destroy(Device* device) {
    for (auto pair : cache) {
      pair.second->destroy(device);
    }
  }
}