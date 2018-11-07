#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/system.h"
#include "render/material.h"
#include "entity/component/mesh.h"
#include "entity/component/camera.h"

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

  // void draw(Entity* camera, Entity* entity);


private:

  GLFWwindow* window;
  
  int screenWidth;

  int screenHeight;

  void draw(glm::mat4 screenProjection, glm::vec3 cameraPosition, glm::mat4 cameraProjection, EntityReference entity);
  
  std::vector<EntityReference> dirLights;

  std::vector<EntityReference> pointLights;

  std::vector<EntityReference> spotlights;

};

#endif //RENDERER_H