#pragma once

#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "asset/asset.h"
#include "render/components/texture.h"
#include "render-vulkan/common.h"
#include "render-vulkan/image.h"

namespace wage {
  namespace render {
    
    class Device;
    class CommandPool;

    class Texture : public asset::Asset {
    public:
      
      VkImageView imageView;
      
      VkSampler sampler;

      Texture(TextureSpec texture);

      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);

      void push(Device* device, CommandPool* commandPool);

      void cleanup(Device* device);

    private:
      
      void transitionImageLayout(Device* device, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

      void copyBufferToImage(Device* device, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

      void createTextureImage(Device* device, VkCommandPool commandPool);

      void createTextureSampler(Device* device);

      bool pushed = false;

      int texWidth;
      int texHeight;
      int texChannels;
      VkDeviceSize imageSize;
      void* pixels;
      Image textureImage;
    };
  }
}