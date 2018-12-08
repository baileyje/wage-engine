#include "render/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wage {

  GlTexture* GlTexture::Default = new GlTexture(new Texture("./resources/textures/default.png"));

  GlTexture::GlTexture(Texture* texture) : loaded(false), texture(texture) {  
  }

  GlTexture::~GlTexture() {
    if (data) {
      stbi_image_free(data);
    }    
  }

  void GlTexture::load(FileSystem* fileSystem) {
    if (loaded) {
      return;
    } 
    GL_FAIL_CHECK(glGenTextures(1, &id));
    File* file = fileSystem->read(texture->getPath());    
    data = stbi_load_from_memory(file->data(), file->length(), &width, &height, &channels, 0); 
    loaded = true;
  }

  void GlTexture::bind() {  
    if (!loaded) {
      return;
    } 
    GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, id));     
    if (!pushed) {      
      GL_FAIL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
      GL_FAIL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));    
      pushed = true;
    }    
  }

  void GlTexture::unbind() {
    GL_FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
  }

}
