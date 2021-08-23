#pragma once

#include <map>
#include <iostream>
#include <vector>

#include "math/vector.h"
#include "asset/manager.h"
#include "memory/buffer.h"
#include "render/components/font.h"

#include "render-vulkan/core/texture.h"
#include "render-vulkan/core/buffer.h"
#include "render-vulkan/text/font/glyph.h"

namespace wage::render::vulkan {

  class Device;

  class CommandPool;

  class Font : public asset::Asset {

  public:
    Font(FontSpec font) : Asset(font), size(font.size()) {}

    Glyph* glyphFor(char c);

    bool onLoad(memory::InputStream* stream, memory::Allocator* allocator);

    void push(Device* device, CommandPool* commandPool);

    void destroy(Device* device);

    VkDescriptorPool descriptorPool;

  private:
    void createDescriptorPool(Device* device, int imageCount);

    int size;

    std::map<char, Glyph> glyphs;

    bool pushed = false;
  };
}