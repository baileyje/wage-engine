#include "render-vulkan/texture_asset.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "memory/buffer.h"

// #include "render-vulkan/device.h"
// #include "render-vulkan/command_pool.h"
// #include "render-vulkan/buffer.h"

namespace wage {
  namespace render {

    TextureAsset::TextureAsset(TextureSpec texture) : Asset(texture) {}

    bool TextureAsset::onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
      auto bufferSize = stream->size();
      auto buffer = (memory::Byte*)malloc(bufferSize);
      stream->read(buffer, bufferSize);
      texture.pixels = stbi_load_from_memory(buffer, bufferSize, &texture.width, &texture.height, &texture.channels, STBI_rgb_alpha);
      free(buffer);
      texture.imageSize = texture.width * texture.height * 4;
      if (!texture.pixels) {
        throw std::runtime_error("failed to load texture image!");
      }
      return true;
    }

    // void Texture::push(Device* device, CommandPool* commandPool) {
    //   if (pushed) return;
    //   createTextureImage(device, commandPool->wrapped);
    //   textureImage.createImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, &imageView);
    //   createTextureSampler(device);
    //   pushed = true;
    // }

    // void Texture::destroy(Device* device) {
    //   if (!pushed) return;
    //   vkDestroySampler(device->logical, sampler, nullptr);
    //   vkDestroyImageView(device->logical, imageView, nullptr);
    //   textureImage.destroy();
    //   pushed = false;
    // }

    // void Texture::createTextureImage(Device* device, VkCommandPool commandPool) {
    //   Buffer stagingBuffer;
    //   device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, imageSize, &stagingBuffer);
    //   stagingBuffer.fillWith(pixels, static_cast<size_t>(imageSize));
    //   stbi_image_free(pixels);      
    //   device->createImage(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImage);
    //   transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    //   copyBufferToImage(device, commandPool, stagingBuffer.buffer, textureImage.image, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    //   transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    //   stagingBuffer.destroy();
    // }

    // void Texture::transitionImageLayout(Device* device, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
    //   VkCommandBuffer commandBuffer = beginSingleTimeCommands(device->logical, commandPool);
    //   VkImageMemoryBarrier barrier{};
    //   barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    //   barrier.oldLayout = oldLayout;
    //   barrier.newLayout = newLayout;
    //   barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    //   barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    //   barrier.image = image;
    //   barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //   barrier.subresourceRange.baseMipLevel = 0;
    //   barrier.subresourceRange.levelCount = 1;
    //   barrier.subresourceRange.baseArrayLayer = 0;
    //   barrier.subresourceRange.layerCount = 1;

    //   VkPipelineStageFlags sourceStage;
    //   VkPipelineStageFlags destinationStage;

    //   if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    //     barrier.srcAccessMask = 0;
    //     barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    //     sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    //     destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    //   } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    //     barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    //     barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    //     sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    //     destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    //   } else {
    //     throw std::invalid_argument("unsupported layout transition!");
    //   }

    //   vkCmdPipelineBarrier(
    //       commandBuffer,
    //       sourceStage, destinationStage,
    //       0,
    //       0, nullptr,
    //       0, nullptr,
    //       1, &barrier);

    //   endSingleTimeCommands(device->logical, commandPool, device->graphicsQueue, commandBuffer);
    // }

    // void Texture::copyBufferToImage(Device* device, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
    //   VkCommandBuffer commandBuffer = beginSingleTimeCommands(device->logical, commandPool);
    //   VkBufferImageCopy region{};
    //   region.bufferOffset = 0;
    //   region.bufferRowLength = 0;
    //   region.bufferImageHeight = 0;
    //   region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    //   region.imageSubresource.mipLevel = 0;
    //   region.imageSubresource.baseArrayLayer = 0;
    //   region.imageSubresource.layerCount = 1;
    //   region.imageOffset = {0, 0, 0};
    //   region.imageExtent = {width, height, 1};
    //   vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    //   endSingleTimeCommands(device->logical, commandPool, device->graphicsQueue, commandBuffer);
    // }

    // void Texture::createTextureSampler(Device* device) {
    //   VkSamplerCreateInfo samplerInfo{};
    //   samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    //   samplerInfo.magFilter = VK_FILTER_LINEAR;
    //   samplerInfo.minFilter = VK_FILTER_LINEAR;
    //   samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    //   samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    //   samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    //   samplerInfo.anisotropyEnable = VK_TRUE;
    //   samplerInfo.maxAnisotropy = 16.0f;
    //   samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    //   samplerInfo.unnormalizedCoordinates = VK_FALSE;
    //   samplerInfo.compareEnable = VK_FALSE;
    //   samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    //   samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    //   if (vkCreateSampler(device->logical, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to create texture sampler!");
    //   }
    // }
  }
}