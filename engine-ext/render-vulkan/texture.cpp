#include "render-vulkan/texture.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wage {
  namespace render {

    bool Texture::onLoad(memory::InputStream* stream, memory::Allocator* allocator) {
      auto bufferSize = stream->size();
      auto buffer = (memory::Byte*)malloc(bufferSize);
      stream->read(buffer, bufferSize);
      pixels = stbi_load_from_memory(buffer, bufferSize, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
      free(buffer);
      imageSize = texWidth * texHeight * 4;
      if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
      }
      return true;
    }

    void Texture::createTextureImage(Device* device, VkCommandPool commandPool) {
      Buffer stagingBuffer;
      device->createBuffer(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, imageSize, &stagingBuffer);
      stagingBuffer.fillWith(pixels, static_cast<size_t>(imageSize));
      stbi_image_free(pixels);      
      device->createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImage);
      transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      copyBufferToImage(device, commandPool, stagingBuffer.buffer, textureImage.image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
      transitionImageLayout(device, commandPool, textureImage.image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
      stagingBuffer.destroy();
    }
  }
}