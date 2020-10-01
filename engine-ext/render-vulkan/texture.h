#pragma once

#include "asset/asset.h"
#include "render-vulkan/common.h"
#include "render-vulkan/image.h"

namespace wage::render {

  class Device;
  class CommandPool;
  class Pipeline;

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

    std::vector<VkDescriptorSet> descriptorSets;

    Texture(VkFormat format = VK_FORMAT_R8G8B8A8_SRGB);

    virtual void push(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool, int imageCount);

    virtual void destroy(Device* device);

  protected:
    void createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool, int imageCount);

    void transitionImageLayout(Device* device, CommandPool* commandPool, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(Device* device, CommandPool* commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createTextureImage(Device* device, CommandPool* commandPool);

    void createTextureSampler(Device* device);

    bool pushed = false;

    Image textureImage;
  };
}