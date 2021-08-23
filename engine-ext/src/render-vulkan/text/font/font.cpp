#include "render-vulkan/text/font/font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "render-vulkan/core/device.h"
#include "render-vulkan/core/command_pool.h"
#include "render-vulkan/core/pipeline.h"
#include "render-vulkan/core/buffer.h"
#include "render-vulkan/core/vertex.h"

namespace wage::render::vulkan {

  Glyph* Font::glyphFor(char c) {
    auto found = glyphs.find(c);
    if (found != glyphs.end()) {
      return &found->second;
    }
    return nullptr;
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
      Glyph glyph(
          math::Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
          math::Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
          face->glyph->advance.x);
      auto size = sizeof(GL_UNSIGNED_BYTE) * glyph.size.x * glyph.size.y;
      glyph.texture.imageSize = size;
      glyph.texture.width = glyph.size.x;
      glyph.texture.height = glyph.size.y;
      if (size > 0) {
        glyph.texture.pixels = malloc(size);
        memcpy(glyph.texture.pixels, face->glyph->bitmap.buffer, size);
      }
      glyphs.insert(std::pair<char, Glyph>(c, glyph));
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
    for (std::pair<char, Glyph> element : glyphs) {
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