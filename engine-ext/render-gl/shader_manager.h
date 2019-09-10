#pragma once

#include <unordered_map>

#include "assets/manager.h"

#include "render-gl/shader.h"

namespace wage { namespace render {

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

    inline void assetManager(assets::Manager* assetManager) {
      _assetManager = assetManager;
    }

  private:
    assets::Manager* _assetManager;

    // std::unordered_map<std::string, GlTexture*> cache;
  };

} }
