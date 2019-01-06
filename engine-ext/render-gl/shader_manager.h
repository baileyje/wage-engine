#ifndef RENDERER_SHADER_MANAGER_H
#define RENDERER_SHADER_MANAGER_H

#include <unordered_map>

#include "engine/assets/manager.h"

#include "engine-ext/render-gl/shader.h"

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

    inline void setAssetManager(AssetManager* assetManager) {
      this->assetManager = assetManager;
    }

  private:

    AssetManager* assetManager;

    // std::unordered_map<std::string, GlTexture*> cache;

  };

}

#endif //RENDERER_SHADER_MANAGER_H