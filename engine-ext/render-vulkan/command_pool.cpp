#include "render-vulkan/command_pool.h"
#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/surface.h"
#include "render-vulkan/model_pipeline.h"
#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {
    CommandPool::CommandPool(Device* device, SwapChain* swapChain) : device(device), swapChain(swapChain) {}

    void CommandPool::create(Surface& surface, ModelPipeline* pipeline) {
      QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device->physical, surface.wrapped);
      VkCommandPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
      poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
      if (vkCreateCommandPool(device->logical, &poolInfo, nullptr, &wrapped) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
      }
      createCommandBuffers();
    }

    void CommandPool::destroy() {
      vkDestroyCommandPool(device->logical, wrapped, nullptr);
    }

    void CommandPool::cleanupBuffers() {
      vkFreeCommandBuffers(device->logical, wrapped, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    }

    void CommandPool::beginCommandBuffer(VkCommandBuffer commandBuffer, ModelPipeline* pipeline, uint32_t imageIndex) {
      vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
      }
      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = pipeline->renderPass;
      renderPassInfo.framebuffer = swapChain->frameBuffers[imageIndex];
      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = swapChain->extent;

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};

      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
      vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->wrapped);
    }

    void CommandPool::endCommandBuffer(VkCommandBuffer commandBuffer) {
      vkCmdEndRenderPass(commandBuffer);
      if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
      }
    }

    void CommandPool::createCommandBuffers() {
      commandBuffers.resize(swapChain->frameBuffers.size());

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.commandPool = wrapped;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

      if (vkAllocateCommandBuffers(device->logical, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
      }
    }
  }
}