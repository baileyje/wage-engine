#pragma once

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/ubo.h"
#include "render-vulkan/texture.h"
#include "render-vulkan/model.h"

#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {

    class CommandPool {

    public:
      CommandPool(Device* device, SwapChain* swapChain) : device(device), swapChain(swapChain), model1("/Users/john/dev/devmode/wage/game/resources/mesh/room.obj"), model2("/Users/john/dev/devmode/wage/game/resources/mesh/cube.obj") {}

      void create(Surface& surface, GraphicsPipeline& pipeline) {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device->physical(), surface.wrapped());
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        if (vkCreateCommandPool(device->logical(), &poolInfo, nullptr, &_wrapped) != VK_SUCCESS) {
          throw std::runtime_error("failed to create command pool!");
        }
        int imageCount = swapChain->images().size();
        model1.create(device, this, pipeline, imageCount);
        model2.create(device, this, pipeline, imageCount);
        createCommandBuffers(pipeline);
      }

      inline void cleanup() {
        vkDestroyCommandPool(device->logical(), _wrapped, nullptr);
      }

      inline void cleanupBuffers() {
        model1.destroy(device, swapChain->images().size());
        model2.destroy(device, swapChain->images().size());        
        vkFreeCommandBuffers(device->logical(), _wrapped, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());        
      }

      inline VkCommandPool wrapped() {
        return _wrapped;
      }

      inline std::vector<VkCommandBuffer>& commandBuffers() {
        return _commandBuffers;
      }

      Model model1;
      Model model2;

    private:
      
      void createCommandBuffers(GraphicsPipeline& pipeline) {
        _commandBuffers.resize(swapChain->frameBuffers().size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _wrapped;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

        if (vkAllocateCommandBuffers(device->logical(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate command buffers!");
        }

        for (size_t i = 0; i < _commandBuffers.size(); i++) {
          VkCommandBufferBeginInfo beginInfo{};
          beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

          if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
          }

          VkRenderPassBeginInfo renderPassInfo{};
          renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
          renderPassInfo.renderPass = pipeline.renderPass();
          renderPassInfo.framebuffer = swapChain->frameBuffers()[i];
          renderPassInfo.renderArea.offset = {0, 0};
          renderPassInfo.renderArea.extent = swapChain->extent();

          std::array<VkClearValue, 2> clearValues{};
          clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
          clearValues[1].depthStencil = {1.0f, 0};

          renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
          renderPassInfo.pClearValues = clearValues.data();

          vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
          vkCmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.wrapped());

          // START: MODEL 1            
          VkBuffer vertexBuffers[] = {model1.vertexBuffer.buffer};
          VkDeviceSize offsets[] = {0};
          // Bind per mesh bucket
          vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);
          vkCmdBindIndexBuffer(_commandBuffers[i], model1.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
          // Apply transform to mesh
          vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout(), 0, 1, &model1.descriptorSets[i], 0, nullptr);
          // Draw.
          vkCmdDrawIndexed(_commandBuffers[i], static_cast<uint32_t>(model1.indices.size()), 1, 0, 0, 0);
          // END: MODEL 1

          // START: MODEL 2
          VkBuffer vertexBuffers2[] = {model2.vertexBuffer.buffer};
          VkDeviceSize offsets2[] = {0};
          // Bind per mesh bucket
          vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers2, offsets2);
          vkCmdBindIndexBuffer(_commandBuffers[i], model2.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
          // Apply transform to mesh
          vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout(), 0, 1, &model2.descriptorSets[i], 0, nullptr);
          // Draw.
          vkCmdDrawIndexed(_commandBuffers[i], static_cast<uint32_t>(model2.indices.size()), 1, 0, 0, 0);
          // END: MODEL 2

          vkCmdEndRenderPass(_commandBuffers[i]);

          if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
          }
        }
      }

      

      Device* device;
      SwapChain* swapChain;

      VkCommandPool _wrapped;
      std::vector<VkCommandBuffer> _commandBuffers;
  
    };

  }
}