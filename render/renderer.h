#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <unordered_map>

#include "core/system.h"
#include "render/material.h"
#include "render/texture.h"
#include "render/vertex_array.h"
#include "render/queue.h"
#include "entity/component/render/mesh.h"
#include "entity/scene.h"
#include "math/matrix.h"
#include "math/vector.h"
#include "render/texture_manager.h"
#include "render/vao_manager.h"

#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

namespace wage {

  class Renderer : public System {

  public:

    Renderer() : System("renderer") { }

    virtual ~Renderer();

    LIFECYCLE_FUNC(init)
    
    LIFECYCLE_FUNC(start)
    
    LIFECYCLE_FUNC(update)
    
    LIFECYCLE_FUNC(stop)
    
    LIFECYCLE_FUNC(deinit)
    
    void draw(Mesh* mesh, GlMaterial* material);

  private:

    GLFWwindow* window;

    FileSystem* fileSystem;
    
    int screenWidth;

    int screenHeight;
    
    std::vector<DirectionalLight*> dirLights;

    std::vector<PointLight*> pointLights;

    std::vector<Spotlight*> spotlights;

    VaoManager vaoManager;

    TextureManager textureManager;
    
  };

}

#endif //RENDERER_H