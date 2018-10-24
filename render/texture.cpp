#include "render/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GlTexture GlTexture::Default(new Texture("./resources/textures/default.png"));


void GlTexture::bind() {
    int width, height, nrChannels;
    // printf("Loading: %s\n", texture->getPath().c_str());
    unsigned char *data = stbi_load(texture->getPath().c_str(), &width, &height, &nrChannels, 0); 
    unsigned int texture;
    FAIL_CHECK(glGenTextures(1, &texture));
    FAIL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
    FAIL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
    FAIL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
    stbi_image_free(data);
}
