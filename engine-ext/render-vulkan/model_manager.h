#pragma once

#include <unordered_map>

#include "asset/manager.h"

#include "render-vulkan/model.h"
#include "render-vulkan/device.h"
#include "render-vulkan/command_pool.h"
#include "render/mesh/mesh.h"
#include "render/mesh/manager.h"

namespace wage {

  namespace render { 

    class ModelManager {
      public:
          Model* load(asset::Manager* assetManager, render::MeshManager* meshManager, MeshSpec meshSpec, TextureSpec textureSpec) {
          auto key = meshSpec.key() + ":" + textureSpec.key();
          if (cache.find(key) != cache.end()) {
            return cache[key];
          }
          Model* model = new Model(meshSpec, textureSpec);
          cache[key] = model;
          model->load(assetManager, meshManager);
          return model;
        }
        std::unordered_map<std::string, Model*> cache;
      };
  }

}