#ifndef RENDERER_TEXTURE_MANAGER_H
#define RENDERER_TEXTURE_MANAGER_H

#include <unordered_map>

#include "fs/file_system.h"

#include "render/gl/texture.h"

namespace wage {

  class GlTextureManager {
  
  public:

    GlTexture* load(Texture* texture) {
      GlTexture* glTexture = GlTexture::Default;      
      if (texture != nullptr) {
        glTexture = cache[texture->getId()];
        if (glTexture == nullptr) {
          glTexture = new GlTexture(texture);
          cache[texture->getId()] = glTexture;
          glTexture->load(fileSystem);
        }
      }
      return glTexture;
    }

    inline void setFileSystem(FileSystem* fileSystem) {
      this->fileSystem = fileSystem;
    }

  private:

    FileSystem* fileSystem;

    std::unordered_map<std::string, GlTexture*> cache;

  };

}

#endif //RENDERER_TEXTURE_MANAGER_H