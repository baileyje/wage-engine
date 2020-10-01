#pragma once

#include "asset/asset.h"
#include "render-vulkan/common.h"
#include "render-vulkan/image.h"

namespace wage {
  namespace render {

    class Device;
    class CommandPool;

    class Texture {
    
    public:
      int width;

      int height;

      int channels;

      VkDeviceSize imageSize;

      void* pixels;
      
      VkImageView imageView;

      VkSampler sampler;

      VkFormat format;
    
      Texture();

      virtual void push(Device* device, CommandPool* commandPool);

      virtual void destroy(Device* device);

    protected:

      void transitionImageLayout(Device* device, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

      void copyBufferToImage(Device* device, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

      void createTextureImage(Device* device, VkCommandPool commandPool);

      void createTextureSampler(Device* device);

      bool pushed = false;
      
      Image textureImage;
    };
  }
}