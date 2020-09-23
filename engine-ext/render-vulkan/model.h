#pragma once

#include <vector>
#include <map>
#include <string>

#include "render-vulkan/vertex.h"
#include "render-vulkan/device.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/texture.h"
#include "render-vulkan/buffer.h"

namespace wage {
  namespace render {

    class CommandPool;

    class Model {
      public:
        Model(std::string meshPath) : meshPath(meshPath) {
        }

        void create(Device* device, CommandPool* commandPool, GraphicsPipeline& pipeline, int imageCount);

        void destroy(Device* device, int imageCount);
        

        Buffer vertexBuffer;
        Buffer indexBuffer;
        std::vector<uint32_t> indices;

        std::vector<Buffer> uniformBuffers;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

      private:
        void loadMesh();

        void createVertexBuffer(Device * device, CommandPool * commandPool);

        void createIndexBuffer(Device * device, CommandPool * commandPool);

        void createUniformBuffers(Device* device, int imageCount);

        void createDescriptorPool(Device* device, int imageCount);

        void createDescriptorSets(Device* device, CommandPool* commandPool, GraphicsPipeline& pipeline, VkDescriptorPool descriptorPool, int imageCount);

        std::string meshPath;

        std::vector<Vertex> vertices;
        // VkDeviceMemory vertexBufferMemory;
        // VkDeviceMemory indexBufferMemory;
        Texture texture;
      };
  }
}