#pragma once

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {

    class CommandPool {

    public:
      CommandPool(Device* device, SwapChain* swapChain) : device(device), swapChain(swapChain) {}

      void create(Surface& surface, GraphicsPipeline& pipeline) {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device->physical(), surface.wrapped());
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        if (vkCreateCommandPool(device->logical(), &poolInfo, nullptr, &wrapped) != VK_SUCCESS) {
          throw std::runtime_error("failed to create command pool!");
        }
        createCommandBuffers(pipeline);
      }

      inline void destroy() {
        vkDestroyCommandPool(device->logical(), wrapped, nullptr);
      }

      inline void cleanupBuffers() {   
        vkFreeCommandBuffers(device->logical(), wrapped, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());        
      }

      VkCommandPool wrapped;

      inline std::vector<VkCommandBuffer>& commandBuffers() {
        return _commandBuffers;
      }

      void beginCommandBuffer(VkCommandBuffer commandBuffer, GraphicsPipeline& pipeline, uint32_t imageIndex) {
        vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
          throw std::runtime_error("failed to begin recording command buffer!");
        }
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = pipeline.renderPass();
        renderPassInfo.framebuffer = swapChain->frameBuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain->extent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.wrapped());
      }

      void endCommandBuffer(VkCommandBuffer commandBuffer) {
        vkCmdEndRenderPass(commandBuffer);
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
          throw std::runtime_error("failed to record command buffer!");
        }
      }

    private:
      
      void createCommandBuffers(GraphicsPipeline& pipeline) {
        _commandBuffers.resize(swapChain->frameBuffers().size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = wrapped;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

        if (vkAllocateCommandBuffers(device->logical(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate command buffers!");
        }
      }

      Device* device;
      SwapChain* swapChain;
      std::vector<VkCommandBuffer> _commandBuffers;  
    };

  }
}