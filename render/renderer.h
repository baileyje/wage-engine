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
#include "entity/component/render/mesh.h"
#include "entity/scene.h"

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

    void draw(glm::mat4 screenProjection, glm::vec3 cameraPosition, glm::mat4 cameraProjection, EntityReference entity);
    
    std::vector<EntityReference> dirLights;

    std::vector<EntityReference> pointLights;

    std::vector<EntityReference> spotlights;

    std::unordered_map<std::string, VertexArray*> vaoCache;

    std::unordered_map<std::string, GlTexture*> textureCache;
    
  };

}

#endif //RENDERER_H