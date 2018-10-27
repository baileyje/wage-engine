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

  void init(Context* context);

  void start(Context* context);

  void update(Context* context);

  void stop(Context* context);

  void deinit(Context* context);

  void draw(Mesh* mesh, GlMaterial* material);

  // void draw(Entity* camera, Entity* entity);


private:

  GLFWwindow* window;
  
  int screenWidth;

  int screenHeight;

  glm::mat4 viewProjectionFrom(Transform* cameraTransform);

  glm::mat4 modelProjectionFrom(Entity* entity);

  glm::mat4 screenProjectionFrom(Camera* camera);

  void draw(glm::mat4 screenProjection, glm::vec3 cameraPosition, glm::mat4 cameraProjection, Entity* entity);
  
  std::vector<Entity*> dirLights;

  std::vector<Entity*> pointLights;

  std::vector<Entity*> spotlights;

};

#endif //RENDERER_H