#ifndef RENDERER_SHADER_MANAGER_H
#define RENDERER_SHADER_MANAGER_H

#include <unordered_map>

#include "assets/manager.h"

#include "render-gl/shader.h"

namespace wage {

  class GlShaderManager {
  
  public:

    GlShader* load(std::string vertextPath, std::string fragmentPath) {
      // TODO: Cache
      // auto shader = new GlShader(
      //   vertextPath,
      //   fragmentPath
      // );
      // shader->load(assetManager);
      // return shader;
      return nullptr;
    }

    inline void assetManager(AssetManager* assetManager) {
      _assetManager = assetManager;
    }

  private:

    AssetManager* _assetManager;

    // std::unordered_map<std::string, GlTexture*> cache;

  };

}

#endif //RENDERER_SHADER_MANAGER_H