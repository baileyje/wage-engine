#pragma once

#include <vector>
#include <map>
#include <string>

#include "render/mesh/manager.h"
#include "asset/manager.h"

#include "render-vulkan/core/vertex.h"
#include "render-vulkan/model/texture_asset.h"
#include "render-vulkan/model/mesh.h"
#include "render-vulkan/model/pipeline.h"
#include "render-vulkan/core/buffer.h"


namespace wage::render::vulkan {

  class CommandPool;
  class Device;
  class Pipeline;

  class Model {
  public:
    Model(MeshSpec meshSpec, TextureSpec textureSpec);

    void load(asset::Manager* assetManager, render::MeshManager* meshManager);

    void push(Device* device, CommandPool* commandPool, ModelPipeline* pipeline, int imageCount);

    void create(Device* device, CommandPool* commandPool, ModelPipeline* pipeline);

    void destroy(Device* device);

    VkDescriptorPool descriptorPool;

    VulkanMesh* mesh = nullptr;

    TextureAsset* textureAsset = nullptr;

    inline bool loaded() {
      return mesh && textureAsset && mesh->loaded() && textureAsset->loaded();
    }

  private:
    void createDescriptorPool(Device* device, int imageCount);

    MeshSpec meshSpec;

    TextureSpec textureSpec;

    bool pushed = false; // Atomic??
  };
}