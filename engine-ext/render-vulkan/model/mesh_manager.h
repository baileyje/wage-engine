#pragma once

#include "render/mesh/manager.h"
#include "asset/manager.h"
#include "render-vulkan/model/mesh.h"

namespace wage::render::vulkan {

  class VulkanMeshManager {
  public:
    void assetManager(asset::Manager* assetManager);

    VulkanMesh* load(MeshSpec spec);

    void destroy(Device* device);

  private:
    std::unordered_map<std::string, VulkanMesh*> cache;

    MeshManager meshManager;
  };
}