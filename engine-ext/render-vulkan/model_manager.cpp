#include "render-vulkan/model_manager.h"

#include "render-vulkan/model.h"
#include "render/mesh/manager.h"

namespace wage {
  namespace render {

    Model* ModelManager::load(asset::Manager* assetManager, MeshManager* meshManager, MeshSpec meshSpec, TextureSpec textureSpec) {
      auto key = meshSpec.key() + ":" + textureSpec.key();
      if (cache.find(key) != cache.end()) {
        return cache[key];
      }
      Model* model = new Model(meshSpec, textureSpec);
      cache[key] = model;
      model->load(assetManager, meshManager);
      return model;
    }

  }
}