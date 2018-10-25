#include "render/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GlTexture GlTexture::Default(new Texture("./resources/textures/default.png"));

GlTexture::GlTexture(Texture* texture) : loaded(false), texture(texture) {  
}

GlTexture::~GlTexture() {
  if (data) {
    stbi_image_free(data);
  }    
}

void GlTexture::load(FileSystem* fileSystem) {
  FAIL_CHECK(glGenTextures(1, &id));
  data = stbi_load(texture->getPath().c_str(), &width, &height, &channels, 0); 
  loaded = true;
}

void GlTexture::bind() {
  if (!loaded) {    
    load(NULL);
  }
  FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, id));
  FAIL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
  FAIL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));    
}
