#pragma once

#include <unordered_map>

#include "asset/manager.h"
#include "render/components/mesh.h"
#include "render/components/texture.h"

namespace wage {

  namespace render { 

    class Model;
    class Device;
    class MeshManager;

    class ModelManager {
      public:
        
        Model* load(asset::Manager* assetManager, MeshManager* meshManager, MeshSpec meshSpec, TextureSpec textureSpec);

        void destroy(Device* device);

      private: 
       
        std::unordered_map<std::string, Model*> cache;
        
      };
  }

}