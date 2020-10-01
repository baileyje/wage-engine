#include "render-vulkan/texture_asset.h"

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "memory/buffer.h"

namespace wage::render::vulkan {

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
}