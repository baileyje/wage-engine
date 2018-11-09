#include "render/renderer.h"

#include <vector>
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "platform/platform.h"

#include "core/logger.h"
#include "core/context.h"

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/vertex_array.h"
#include "render/util.h"
#include "render/material.h"
#include "render/texture.h"
#include "render/shader/shader.h"

#include "entity/component/mesh.h"
#include "entity/component/material.h"
#include "entity/component/perspective_camera.h"
#include "entity/component/orthographic_camera.h"
#include "entity/component/directional_light.h"
#include "entity/component/point_light.h"
#include "entity/component/spotlight.h"


Renderer::~Renderer() {
}

void Renderer::init(Context* context)  {
  Logger::info("Initializing Renderer.");
}

void Renderer::start(Context* context) {
  Logger::info("Starting Renderer.");
  Platform* platform = context->get<Platform>();
  window = platform->getWindow();
  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  GL_FAIL_CHECK(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
  Shader::initDefault(context->get<FileSystem>());  
  
  // TODO: Listen for changes to entities and figure out how to get these!!!
  EntityManager* manager = context->get<EntityManager>();
  dirLights = manager->with("DirectionalLight");
  pointLights = manager->with("PointLight");
  spotlights = manager->with("Spotlight");

  glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
}

void Renderer::update(Context* context) {  
  GL_FAIL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
  GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  EntityManager* manager = context->get<EntityManager>();
  auto cameraEntity = manager->with("Camera")[0];
  assert(cameraEntity);
  Transform* cameraTransform = cameraEntity->getTransform();
  glm::vec3 cameraPosition = cameraTransform->getPosition();
  glm::mat4 cameraProjection = Camera::viewProjectionFor(cameraTransform);
  Camera* camera = (Camera*)cameraEntity->getComponents()->get("Camera");
  glm::mat4 screenProjection = camera->screenProjection(Vector2(screenWidth, screenHeight));
  for (EntityReference entity : manager->with("Mesh")) {
    draw(screenProjection, cameraPosition, cameraProjection, entity);
    // for (auto child : *entity->getChildren()) {
    //   draw(screenProjection, cameraPosition, cameraProjection, child);
    // }
  }
  GL_FAIL_CHECK(glfwSwapBuffers(window));
  GL_FAIL_CHECK(glfwPollEvents());
}


void Renderer::draw(glm::mat4 screenProjection, glm::vec3 cameraPosition, glm::mat4 cameraProjection, EntityReference entity) {
  Mesh* mesh = entity->get<Mesh>();
  if (!mesh) {    
    return;
  }
  GlMaterial material(Shader::Default);
  material.bind();
  glm::mat4 model = entity->getTransform()->worldPorjection();
  material.setMat4("model", model);
  material.setMat4("view", cameraProjection);
  material.setMat4("projection", screenProjection);  
  
  material.setVec3("viewPos", cameraPosition);
  material.setInt("numDirLights", dirLights.size());

  int idx = 0;
  for (auto dirLightEnt : dirLights) {
    std::stringstream base;
    base << "dirLights[" << idx++ << "]";
    DirectionalLight* light = dirLightEnt->get<DirectionalLight>();
    Vector cameraEulers = glm::eulerAngles(dirLightEnt->getTransform()->getRotation());
    material.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
  }
  
  material.setInt("numPointLights", pointLights.size());
  idx = 0;
  for (auto lightEnt : pointLights) {
    std::stringstream base;
    base << "pointLights[" << idx++ << "]";
    PointLight* light = lightEnt->get<PointLight>();
    material.setVec3(base.str() + ".position", lightEnt->getTransform()->getPosition());
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
    material.setFloat(base.str() + ".constant", light->getConstant());
    material.setFloat(base.str() + ".linear", light->getLinear());
    material.setFloat(base.str() + ".quadratic", light->getQuadratic());
  }

  material.setInt("numSpotLights", spotlights.size());
  idx = 0;
  for (auto lightEnt : spotlights) {
    std::stringstream base;
    base << "spotLights[" << idx++ << "]";
    Spotlight* light = lightEnt->get<Spotlight>();
    material.setVec3(base.str() + ".position", lightEnt->getTransform()->getPosition());
    Vector cameraEulers = glm::eulerAngles(lightEnt->getTransform()->getRotation());
    material.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
    material.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
    material.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
    material.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
    material.setFloat(base.str() + ".constant", light->getConstant());
    material.setFloat(base.str() + ".linear", light->getLinear());
    material.setFloat(base.str() + ".quadratic", light->getQuadratic());
    material.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->getCutOff())));
    material.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->getOuterCutOff())));
  }

  GlTexture::Default.bind();
  material.setInt("material.diffuse", 0);
  material.setFloat("material.shininess", 32.0f);
  draw(mesh, &material);
}


// void Renderer::draw(Entity* camera, Entity* entity) {  
//   Transform* cameraTransform = camera->getTransform();
//   glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());  
//   draw(cameraPosition, viewProjectionFrom(camera), entity);
// }

void Renderer::draw(Mesh* mesh, GlMaterial* material) {
  VertexArray vao;
  vao.bind();  
  material->bind();

  // TODO: I think this could be cached for sure.  
  // Create Verts Buff
  VertexBuffer verts(mesh->getVertices()->data(), mesh->getVertices()->size() * 3 * sizeof(float));
  verts.getLayout()->pushFloat(3);
  vao.addBuffer(&verts);
  // Create Norms Buff
  VertexBuffer norms(mesh->getNormals()->data(), mesh->getNormals()->size() * 3 * sizeof(float));
  norms.getLayout()->pushFloat(3);
  vao.addBuffer(&norms);

  VertexBuffer uvs(mesh->getUvs()->data(), mesh->getUvs()->size() * 3 * sizeof(float));
  uvs.getLayout()->pushFloat(2);
  vao.addBuffer(&uvs);  
  // Create Index Buff
  IndexBuffer indices((const unsigned int*)mesh->getIndices()->data(), mesh->getIndices()->size());
  indices.bind();
  GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
  // GL_FAIL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));
  indices.unbind();
  norms.unbind();
  verts.unbind();
  material->unbind();
  vao.unbind();
}

void Renderer::stop(Context* context) {
  Logger::info("Stopping Renderer.");
}

void Renderer::deinit(Context* context) {
  Logger::info("Deinitializing Renderer.");
}

