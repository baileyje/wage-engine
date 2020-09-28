#include "render-vulkan/command_pool.h"

#include "render-vulkan/context.h"
#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {
    CommandPool::CommandPool(VulkanContext* context) : context(context) {}

    void CommandPool::create() {
      QueueFamilyIndices queueFamilyIndices = findQueueFamilies(context->device.physical, context->surface.wrapped);
      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      if (vkCreateCommandPool(context->device.logical, &poolInfo, nullptr, &wrapped) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
      }
      createCommandBuffers();
    }

    void CommandPool::destroy() {
      vkDestroyCommandPool(context->device.logical, wrapped, nullptr);
    }

    void CommandPool::cleanupBuffers() {
      vkFreeCommandBuffers(context->device.logical, wrapped, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }

    VkCommandBuffer CommandPool::beginCommandBuffer(uint32_t imageIndex) {
      auto commandBuffer = commandBuffers[imageIndex];
      vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
      }
      return commandBuffer;
    }

    void CommandPool::endCommandBuffer(VkCommandBuffer commandBuffer) {
      
      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
      }
    }

    void CommandPool::createCommandBuffers() {
      commandBuffers.resize(context->swapChain.frameBuffers.size());
      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = wrapped;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
      if (vkAllocateCommandBuffers(context->device.logical, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
      }
    }
  }
}