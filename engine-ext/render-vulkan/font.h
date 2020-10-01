#pragma once

#include <map>
#include <iostream>
#include <vector>

#include "math/vector.h"
#include "asset/manager.h"
#include "memory/buffer.h"
#include "render/components/font.h"

#include "render-vulkan/texture.h"
#include "render-vulkan/buffer.h"
#include "render-vulkan/font_glyph.h"

namespace wage::render {

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