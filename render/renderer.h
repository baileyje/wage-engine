#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/system.h"
#include "render/material.h"
#include "entity/component/mesh.h"

class Renderer : public System {

public:

  Renderer() : System("renderer") { }

  virtual ~Renderer();

  void init(Context* context);

  void start(Context* context);

  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

  void draw(Mesh* mesh, GlMaterial* material);

  void draw(Entity* camera, Entity* entity);

private:

  GLFWwindow* window;
  
  glm::mat4 screenProjection;

  glm::mat4 viewProjectionFrom(Entity* camera);

  glm::mat4 modelProjectionFrom(Entity* entity);

  void draw(glm::vec3 cameraPosition, glm::mat4 cameraProjection, Entity* entity);
  
  std::vector<Entity*> dirLights;

  std::vector<Entity*> pointLights;

  std::vector<Entity*> spotlights;

};

#endif //RENDERER_H