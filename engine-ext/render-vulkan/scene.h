#pragma once 

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/model_pipeline.h"
#include "render-vulkan/command_pool.h"

namespace wage {

  namespace render {

    class VulkanScene {
      public:
        void create(Device* device, CommandPool* commandPool, ModelPipeline* pipeline, int imageCount);

        void destroy(Device* device);

        void createUniformBuffers(Device* device, int imageCount);

        void createDescriptorPool(Device* device, int imageCount);

        void createDescriptorSets(Device* device, CommandPool* commandPool, ModelPipeline* pipeline, int imageCount);

        std::vector<Buffer> uniformBuffers;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
    };

  }

}