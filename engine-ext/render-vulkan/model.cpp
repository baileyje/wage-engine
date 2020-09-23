#include "render-vulkan/model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "render-vulkan/command_pool.h"

namespace wage {
  namespace render {

    void Model::create(Device* device, CommandPool* commandPool, GraphicsPipeline& pipeline, int imageCount) {
      loadMesh();
      createVertexBuffer(device, commandPool);
      createIndexBuffer(device, commandPool);
      texture.create(device, commandPool->wrapped());
      createUniformBuffers(device, imageCount);
      createDescriptorPool(device, imageCount);
      createDescriptorSets(device, commandPool, pipeline, descriptorPool, imageCount);      
    }

    void Model::destroy(Device* device, int imageCount) {
      indexBuffer.destroy();
      vertexBuffer.destroy();
      texture.cleanup(device);
      for (size_t i = 0; i < imageCount; i++) {
        uniformBuffers[i].destroy();
      }
      vkDestroyDescriptorPool(device->logical(), descriptorPool, nullptr);
    }

    void Model::loadMesh() {
          tinyobj::attrib_t attrib;
          std::vector<tinyobj::shape_t> shapes;
          std::vector<tinyobj::material_t> materials;
          std::string warn, err;
          if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshPath.c_str())) {
            throw std::runtime_error(warn + err);
          }
          std::unordered_map<Vertex, uint32_t> uniqueVertices{};
          for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
              Vertex vertex{};
              vertex.pos = {
                  attrib.vertices[3 * index.vertex_index + 0],
                  attrib.vertices[3 * index.vertex_index + 1],
                  attrib.vertices[3 * index.vertex_index + 2]};
              vertex.texCoord = {
                  attrib.texcoords[2 * index.texcoord_index + 0],
                  1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
              vertex.color = {1.0f, 1.0f, 1.0f};
              if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
              }
              indices.push_back(uniqueVertices[vertex]);
            }
          }
        }

    void Model::createVertexBuffer(Device* device, CommandPool* commandPool) {
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
      
      stagingBuffer.fillWith(vertices.data(), (size_t)bufferSize);
      
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &vertexBuffer);

      copyBuffer(device->logical(), commandPool->wrapped(), device->graphicsQueue(), stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);

      stagingBuffer.destroy();
    }

    void Model::createIndexBuffer(Device* device, CommandPool* commandPool) {
      VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);

      stagingBuffer.fillWith(indices.data(), (size_t)bufferSize);
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
      
      copyBuffer(device->logical(), commandPool->wrapped(), device->graphicsQueue(), stagingBuffer.buffer, indexBuffer.buffer, bufferSize);

      stagingBuffer.destroy();
    }


      void Model::createUniformBuffers(Device* device, int imageCount) {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        uniformBuffers.resize(imageCount);
        for (size_t i = 0; i < imageCount; i++) {
          device->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &uniformBuffers[i]);
        }
      }

      void Model::createDescriptorPool(Device* device, int imageCount) {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(imageCount);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(imageCount);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(imageCount);

        if (vkCreateDescriptorPool(device->logical(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
          throw std::runtime_error("failed to create descriptor pool!");
        }
      }

      void Model::createDescriptorSets(Device* device, CommandPool* commandPool, GraphicsPipeline& pipeline, VkDescriptorPool descriptorPool, int imageCount) {
        std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline.descriptorSetLayout());
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(imageCount);
        if (vkAllocateDescriptorSets(device->logical(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
          throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < imageCount; i++) {
          VkDescriptorBufferInfo bufferInfo{};
          bufferInfo.buffer = uniformBuffers[i].buffer;
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

    
  }
}