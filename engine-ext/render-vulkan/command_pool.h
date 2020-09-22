#pragma once

#include "render-vulkan/common.h"
#include "render-vulkan/device.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/ubo.h"
#include "render-vulkan/texture.h"

#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {

    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4};
         
    class CommandPool {

    public:
      CommandPool(Device* device, SwapChain* swapChain) : device(device), swapChain(swapChain) {}

      void create(Surface& surface, GraphicsPipeline& pipeline) {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device->physical(), surface.wrapped());
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        if (vkCreateCommandPool(device->logical(), &poolInfo, nullptr, &_wrapped) != VK_SUCCESS) {
          throw std::runtime_error("failed to create command pool!");
        }
        texture.create(device, _wrapped);
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets(pipeline);
        createCommandBuffers(pipeline);
      }

      inline void cleanup() {
        vkDestroyCommandPool(device->logical(), _wrapped, nullptr);
      }

      inline void cleanupBuffers() {
        vkDestroyBuffer(device->logical(), indexBuffer, nullptr);
        vkFreeMemory(device->logical(), indexBufferMemory, nullptr);
        vkDestroyBuffer(device->logical(), vertexBuffer, nullptr);
        vkFreeMemory(device->logical(), vertexBufferMemory, nullptr);
        vkFreeCommandBuffers(device->logical(), _wrapped, static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
        // TODO: WADDAFUK
        for (size_t i = 0; i < swapChain->images().size(); i++) {
          vkDestroyBuffer(device->logical(), uniformBuffers[i], nullptr);
          vkFreeMemory(device->logical(), _uniformBuffersMemory[i], nullptr);
        }
        vkDestroyDescriptorPool(device->logical(), descriptorPool, nullptr);
        texture.cleanup(device);
      }

      inline std::vector<VkCommandBuffer>& commandBuffers() {
        return _commandBuffers;
      }

      inline std::vector<VkDeviceMemory>& uniformBuffersMemory() {
        return _uniformBuffersMemory;
      }

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

          VkBuffer vertexBuffers[] = {vertexBuffer};
          VkDeviceSize offsets[] = {0};
          vkCmdBindVertexBuffers(_commandBuffers[i], 0, 1, vertexBuffers, offsets);
          vkCmdBindIndexBuffer(_commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
          vkCmdBindDescriptorSets(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.layout(), 0, 1, &descriptorSets[i], 0, nullptr);
          vkCmdDrawIndexed(_commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

          vkCmdEndRenderPass(_commandBuffers[i]);

          if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
          }
        }
      }

      void createVertexBuffer() {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(device->logical(), device->physical(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device->logical(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(device->logical(), stagingBufferMemory);

        createBuffer(device->logical(), device->physical(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(device->logical(), _wrapped, device->graphicsQueue(), stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device->logical(), stagingBuffer, nullptr);
        vkFreeMemory(device->logical(), stagingBufferMemory, nullptr);
      }

      void createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(device->logical(), device->physical(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device->logical(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device->logical(), stagingBufferMemory);

        createBuffer(device->logical(), device->physical(), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(device->logical(), _wrapped, device->graphicsQueue(), stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device->logical(), stagingBuffer, nullptr);
        vkFreeMemory(device->logical(), stagingBufferMemory, nullptr);
      }

      void createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(swapChain->images().size());
        _uniformBuffersMemory.resize(swapChain->images().size());

        for (size_t i = 0; i < swapChain->images().size(); i++) {
          createBuffer(device->logical(), device->physical(), bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], _uniformBuffersMemory[i]);
        }
      }

      void createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChain->images().size());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChain->images().size());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(swapChain->images().size());

        if (vkCreateDescriptorPool(device->logical(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
          throw std::runtime_error("failed to create descriptor pool!");
        }
      }

      void createDescriptorSets(GraphicsPipeline& pipeline) {
        std::vector<VkDescriptorSetLayout> layouts(swapChain->images().size(), pipeline.descriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChain->images().size());
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(swapChain->images().size());
        if (vkAllocateDescriptorSets(device->logical(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < swapChain->images().size(); i++) {
          VkDescriptorBufferInfo bufferInfo{};
          bufferInfo.buffer = uniformBuffers[i];
          bufferInfo.offset = 0;
          bufferInfo.range = sizeof(UniformBufferObject);

          VkDescriptorImageInfo imageInfo{};
          imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          imageInfo.imageView = texture.imageView();
          imageInfo.sampler = texture.sampler();

          std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

          descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptorWrites[0].dstSet = descriptorSets[i];
          descriptorWrites[0].dstBinding = 0;
          descriptorWrites[0].dstArrayElement = 0;
          descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          descriptorWrites[0].descriptorCount = 1;
          descriptorWrites[0].pBufferInfo = &bufferInfo;

          descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptorWrites[1].dstSet = descriptorSets[i];
          descriptorWrites[1].dstBinding = 1;
          descriptorWrites[1].dstArrayElement = 0;
          descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptorWrites[1].descriptorCount = 1;
          descriptorWrites[1].pImageInfo = &imageInfo;

          vkUpdateDescriptorSets(device->logical(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
      }

      Device* device;
      SwapChain* swapChain;

      VkCommandPool _wrapped;
      std::vector<VkCommandBuffer> _commandBuffers;

      Texture texture;

      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;
      VkBuffer indexBuffer;
      VkDeviceMemory indexBufferMemory;

      std::vector<VkBuffer> uniformBuffers;
      std::vector<VkDeviceMemory> _uniformBuffersMemory;

      VkDescriptorPool descriptorPool;
      std::vector<VkDescriptorSet> descriptorSets;
    };

  }
}