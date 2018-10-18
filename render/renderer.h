#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/system.h"
#include "render/material.h"
#include "render/mesh/mesh.h"

class Renderer : public System {

public:

  Renderer() : System("renderer") { }

  virtual ~Renderer();

  void init(Context* context);

  void start(Context* context);

  void fixedUpdate(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

  void draw(Mesh* mesh, Material* material);

  void draw(Camera* camera, Entity* entity);

private:

  GLFWwindow* window;
  
  glm::mat4 screenProjection;

  glm::mat4 viewProjectionFrom(Camera* camera);

  glm::mat4 modelProjectionFrom(Entity* entity);

  void draw(glm::vec3 cameraPosition, glm::mat4 cameraProjection, Entity* entity);
  
};

#endif //RENDERER_H