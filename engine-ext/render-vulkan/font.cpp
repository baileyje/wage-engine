#include "render-vulkan/font.h"

#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/pipeline.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/vertex.h"

namespace wage {
  namespace render {

    Character::Character(math::Vector2 size, math::Vector2 bearing, unsigned int advance) : size(size), bearing(bearing), advance(advance), pushed(false) {}

    void Character::push(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool) {
      if (pushed) {
        return;
      }
      if (imageSize > 0) {
        createTextureImage(device, commandPool->wrapped);
        textureImage.createImageView(VK_FORMAT_R8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, &imageView);
        createTextureSampler(device);
      }
      createDescriptorSets(device, commandPool, pipeline, descriptorPool, commandPool->commandBuffers.size());
      // Need to tighten this up big time
      std::vector<Vertex> vertices = {
          {{size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}}, // top right
          {{size.x, 0.0, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},    // bottom right
          {{0.0, 0.0, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},       // bottom left
          {{0.0, size.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}}     // top left
      };
      std::vector<uint32_t> indices = {
          0, 1, 3, // first Triangle
          1, 2, 3  // second Triangle
      };
      createVertexBuffer(device, commandPool, vertices);
      createIndexBuffer(device, commandPool, indices);
      pushed = true;
    }

    void Character::destroy(Device* device) {
      if (!pushed) return;
      vkDestroySampler(device->logical, sampler, nullptr);
      vkDestroyImageView(device->logical, imageView, nullptr);
      textureImage.destroy();
      vertexBuffer.destroy();
      indexBuffer.destroy();
      pushed = false;
    }

    Character* Font::characterFor(char c) {
      auto found = characters.find(c);
      if (found != characters.end()) {
        return &found->second;
      }
      return nullptr;
    }

    void Character::createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool, int imageCount) {
      std::vector<VkDescriptorSetLayout> layouts(imageCount, pipeline->textureLayout);
      VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptorPool;
      allocInfo.descriptorSetCount = static_cast<uint32_t>(imageCount);
      allocInfo.pSetLayouts = layouts.data();

      descriptorSets.resize(imageCount);
      if (vkAllocateDescriptorSets(device->logical, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
      }

      for (size_t i = 0; i < imageCount; i++) {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = imageView;
        imageInfo.sampler = sampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device->logical, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
      }
    }

    void Character::createTextureImage(Device* device, VkCommandPool commandPool) {
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, imageSize, &stagingBuffer);
      stagingBuffer.fillWith(pixels, static_cast<size_t>(imageSize));
      free(pixels);
      device->createImage(size.x, size.y, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImage);
      transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      copyBufferToImage(device, commandPool, stagingBuffer.buffer, textureImage.image, static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y));
      transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
      stagingBuffer.destroy();
    }

    void Character::transitionImageLayout(Device* device, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands(device->logical, commandPool);
      VkImageMemoryBarrier barrier{};
      barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      barrier.oldLayout = oldLayout;
      barrier.newLayout = newLayout;
      barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barrier.image = image;
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      barrier.subresourceRange.baseMipLevel = 0;
      barrier.subresourceRange.levelCount = 1;
      barrier.subresourceRange.baseArrayLayer = 0;
      barrier.subresourceRange.layerCount = 1;

      VkPipelineStageFlags sourceStage;
      VkPipelineStageFlags destinationStage;

      if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      } else {
        throw std::invalid_argument("unsupported layout transition!");
      }

      vkCmdPipelineBarrier(
          commandBuffer,
          sourceStage, destinationStage,
          0,
          0, nullptr,
          0, nullptr,
          1, &barrier);

      endSingleTimeCommands(device->logical, commandPool, device->graphicsQueue, commandBuffer);
    }

    void Character::copyBufferToImage(Device* device, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands(device->logical, commandPool);
      VkBufferImageCopy region{};
      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;
      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = 1;
      region.imageOffset = {0, 0, 0};
      region.imageExtent = {width, height, 1};
      vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
      endSingleTimeCommands(device->logical, commandPool, device->graphicsQueue, commandBuffer);
    }

    void Character::createTextureSampler(Device* device) {
      VkSamplerCreateInfo samplerInfo{};
      samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
      samplerInfo.anisotropyEnable = VK_TRUE;
      samplerInfo.maxAnisotropy = 16.0f;
      samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
      samplerInfo.unnormalizedCoordinates = VK_FALSE;
      samplerInfo.compareEnable = VK_FALSE;
      samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

      if (vkCreateSampler(device->logical, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
      }
    }

    void Character::createVertexBuffer(Device* device, CommandPool* commandPool, std::vector<Vertex> vertices) {
      VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
      stagingBuffer.fillWith(vertices.data(), (size_t)bufferSize);
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &vertexBuffer);
      copyBuffer(device->logical, commandPool->wrapped, device->graphicsQueue, stagingBuffer.buffer, vertexBuffer.buffer, bufferSize);
      stagingBuffer.destroy();
    }

    void Character::createIndexBuffer(Device* device, CommandPool* commandPool, std::vector<uint32_t> indices) {
      VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, bufferSize, &stagingBuffer);
      stagingBuffer.fillWith(indices.data(), (size_t)bufferSize);
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, bufferSize, &indexBuffer);
      copyBuffer(device->logical, commandPool->wrapped, device->graphicsQueue, stagingBuffer.buffer, indexBuffer.buffer, bufferSize);
      stagingBuffer.destroy();
    }

    bool Font::onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
      FT_Library freeType;
      if (FT_Init_FreeType(&freeType))
        core::Logger::error("FREETYPE: Could not init FreeType Library");
      FT_Face face;
      auto bufferSize = stream->size();
      auto buffer = (memory::Byte*)malloc(bufferSize);
      stream->read(buffer, bufferSize);
      if (FT_New_Memory_Face(freeType, buffer, bufferSize, 0, &face))
        core::Logger::error("FREETYPE: Failed to load font");
      free(buffer);
      FT_Set_Pixel_Sizes(face, 0, size);
      for (uint8 c = 0; c < 128; c++) {
        FT_Error error = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (error) {
          printf("Error: %d\n", error);
          core::Logger::error("FREETYTPE: Failed to load Glyph");
          continue;
        }
        Character character(
          math::Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows), 
          math::Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top), 
          face->glyph->advance.x
        );
        auto size = sizeof(GL_UNSIGNED_BYTE) * character.size.x * character.size.y;
        character.imageSize = size;
        if (size > 0) {
          character.pixels = malloc(size);
          memcpy(character.pixels, face->glyph->bitmap.buffer, size);
        }
        characters.insert(std::pair<char, Character>(c, character));
      }
      FT_Done_Face(face);
      FT_Done_FreeType(freeType);
      return true;
    }

    void Font::push(Device* device, CommandPool* commandPool) {
      if (pushed) return;
      createDescriptorPool(device, commandPool->commandBuffers.size());
      pushed = true;
    }

    void Font::destroy(Device* device) {      
      for (std::pair<char, Character> element : characters) {
        element.second.destroy(device);
      }
      if (!pushed) return;
      vkDestroyDescriptorPool(device->logical, descriptorPool, nullptr);
      pushed = false;
    }

    void Font::createDescriptorPool(Device* device, int imageCount) {
      std::array<VkDescriptorPoolSize, 2> poolSizes{};
      poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSizes[0].descriptorCount = static_cast<uint32_t>(imageCount);
      poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSizes[1].descriptorCount = static_cast<uint32_t>(imageCount * 200);

      VkDescriptorPoolCreateInfo poolInfo{};
      poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
      poolInfo.pPoolSizes = poolSizes.data();
      poolInfo.maxSets = static_cast<uint32_t>(imageCount * 200);

      if (vkCreateDescriptorPool(device->logical, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
      }
    }
  }
}