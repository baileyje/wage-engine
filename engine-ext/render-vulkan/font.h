#pragma once

#include <map>
#include <iostream>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "math/vector.h"
#include "asset/manager.h"
#include "memory/buffer.h"
#include "render/components/font.h"

#include "render-vulkan/texture.h"
#include "render-vulkan/buffer.h"

namespace wage {
  namespace render {

    class Device;
    class CommandPool;
    class Pipeline;
    struct Vertex;

    class Character {

    public:
      Character(math::Vector2 size, math::Vector2 bearing, unsigned int advance);

      void push(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool);

      void destroy(Device* device);

      math::Vector2 size;

      math::Vector2 bearing;

      unsigned int advance;

      VkDeviceSize imageSize;

      void* pixels;
      
      bool pushed = false;

      std::vector<VkDescriptorSet> descriptorSets;

      VkImageView imageView;

      VkSampler sampler;

      Image textureImage;

      Buffer vertexBuffer;

      Buffer indexBuffer;

    private:
      void createDescriptorSets(Device* device, CommandPool* commandPool, Pipeline* pipeline, VkDescriptorPool descriptorPool, int imageCount);

      void transitionImageLayout(Device* device, VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

      void copyBufferToImage(Device* device, VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

      void createTextureImage(Device* device, VkCommandPool commandPool);

      void createTextureSampler(Device* device);

      void createVertexBuffer(Device* device, CommandPool* commandPool, std::vector<Vertex> vertices);

      void createIndexBuffer(Device* device, CommandPool* commandPool, std::vector<uint32_t> indices);
    };

    class Font : public asset::Asset {

    public:
      Font(FontSpec font) : Asset(font), size(font.size()) {}

      Character* characterFor(char c);

      bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);
      
      void push(Device* device, CommandPool* commandPool);

      void destroy(Device* device);

      VkDescriptorPool descriptorPool;

    private:
      void createDescriptorPool(Device* device, int imageCount);      

      int size;

      std::map<char, Character> characters;

      bool pushed = false;
    };

  }
}