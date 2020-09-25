#pragma once

#include <vector>
#include <map>
#include <string>

#include "render/mesh/manager.h"
#include "asset/manager.h"

#include "render-vulkan/vertex.h"
#include "render-vulkan/device.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/texture.h"
#include "render-vulkan/mesh.h"
#include "render-vulkan/buffer.h"

namespace wage {
  namespace render {

    class CommandPool;

    class Model {
      public:
        Model(MeshSpec meshSpec, TextureSpec textureSpec) : meshSpec(meshSpec), textureSpec(textureSpec), pushed(false) {
        }

        void load(asset::Manager* assetManager, render::MeshManager* meshManager) {
          if (loaded()) return;
          mesh = new VulkanMesh(meshManager->load(meshSpec));
          texture = assetManager->load<Texture>(textureSpec);
        }

        void push(Device* device, CommandPool* commandPool, GraphicsPipeline* pipeline, int imageCount) {
          if (pushed) return;
          mesh->push(device, commandPool);
          texture->push(device, commandPool);
          createUniformBuffers(device, imageCount);
          createDescriptorPool(device, imageCount);
          createDescriptorSets(device, commandPool, pipeline, imageCount);
          pushed = true;
        }

        void create(Device* device, CommandPool* commandPool, GraphicsPipeline* pipeline, int imageCount);

        void destroy(Device* device, int imageCount);

        std::vector<Buffer> uniformBuffers;

        VkDescriptorPool descriptorPool;

        std::vector<VkDescriptorSet> descriptorSets;

        VulkanMesh* mesh = nullptr;

        Texture* texture = nullptr;

        inline bool loaded() {
          return mesh && texture && mesh->loaded() && texture->loaded();
        }

      private:

        void createUniformBuffers(Device* device, int imageCount);

        void createDescriptorPool(Device* device, int imageCount);

        void createDescriptorSets(Device* device, CommandPool* commandPool, GraphicsPipeline* pipeline, int imageCount);

        MeshSpec meshSpec;

        TextureSpec textureSpec;

        bool pushed; // Atomic??
      };
  }
}