#include "render-vulkan/command_pool.h"

#include "render-vulkan/context.h"
#include "render-vulkan/vertex.h"

namespace wage::render {
  
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

  VkCommandBuffer CommandPool::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = wrapped;
    allocInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(context->device.logical, &allocInfo, &commandBuffer);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
  }

  void CommandPool::endSingleTimeCommands(VkQueue graphicsQueue, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(context->device.logical, wrapped, 1, &commandBuffer);
  }

  void CommandPool::copyBuffer(VkQueue graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();
    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
    endSingleTimeCommands(graphicsQueue, commandBuffer);
  }
}