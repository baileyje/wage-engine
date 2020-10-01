#pragma once

#include "render-vulkan/common.h"

namespace wage::render {

  class VulkanContext;

  class CommandPool {

  public:
    VkCommandPool wrapped;

    std::vector<VkCommandBuffer> commandBuffers;

    CommandPool(VulkanContext* context);

    void create();

    void destroy();

    void cleanupBuffers();

    VkCommandBuffer beginCommandBuffer(uint32_t imageIndex);

    void endCommandBuffer(VkCommandBuffer commandBuffer);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkQueue graphicsQueue, VkCommandBuffer commandBuffer);

    void copyBuffer(VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  private:
    void createCommandBuffers();

    VulkanContext* context;
  };

}