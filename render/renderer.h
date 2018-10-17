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
  
  glm::mat4 screenProjection; // TODO: should come from camera

};

#endif //RENDERER_H