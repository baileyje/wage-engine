#include "render-gl/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wage {

  GlTexture::GlTexture(Texture* texture) : Asset(texture->getPath()), pushed(false), texture(texture) {  
  }

  GlTexture::~GlTexture() {
    if (data) {
      stbi_image_free(data);
    }    
  }

  void GlTexture::onLoad(Buffer* buffer) {
    data = stbi_load_from_memory(buffer->data(), buffer->length(), &width, &height, &channels, 0);
  }

  void GlTexture::push() {
    GL_FAIL_CHECK(glGenTextures(1, &id));
    GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, id));         
    GL_FAIL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    GL_FAIL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));    
    pushed = true;
  }

  void GlTexture::bind() {  
    if (!loaded()) {
      return;
    } 
    if (!pushed) {
      push();
    }
    GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, id));
  }

  void GlTexture::unbind() {
    GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  }

}
