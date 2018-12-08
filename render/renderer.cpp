#include "render/renderer.h"

#include <vector>
#include <sstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "platform/platform.h"

#include "core/logger.h"
#include "core/system/context.h"

#include "render/vertex_buffer.h"
#include "render/index_buffer.h"
#include "render/util.h"
#include "render/material.h"
#include "render/shader/shader.h"

#include "entity/component/render/mesh.h"
#include "entity/component/render/material.h"
#include "entity/component/camera/perspective_camera.h"
#include "entity/component/camera/orthographic_camera.h"
#include "entity/component/lighting/directional_light.h"
#include "entity/component/lighting/point_light.h"
#include "entity/component/lighting/spotlight.h"

namespace wage {

  Renderer::~Renderer() {
  }

  void Renderer::init(SystemContext* context)  {
  }

  void Renderer::start(SystemContext* context) {
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
    
    const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    Logger::info("Graphics Vendor: ", vendor);
    Logger::info("Graphics Renderer: ", renderer);
    // Intialize default render assets
    fileSystem = context->get<FileSystem>();
    Shader::initDefault(fileSystem);  
    GlTexture::Default->load(fileSystem);
    
    // TODO: Listen for changes to entities and figure out how to get these!!!
    EntityManager* manager = context->get<EntityManager>();
    dirLights = manager->with("DirectionalLight");
    // pointLights = manager->with("PointLight");
    // spotlights = manager->with("Spotlight");

    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
  }

  void Renderer::update(SystemContext* context) {  
    GL_FAIL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
    GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    EntityManager* manager = context->get<EntityManager>();
    auto cameraEntities = manager->with("Camera");
    if (!cameraEntities.size()) {
      Logger::error("No Camera");
      return;
    }
    auto cameraEntity = cameraEntities[0];
    assert(cameraEntity);
    Transform cameraTransform = cameraEntity->getTransform();
    glm::vec3 cameraPosition = cameraTransform.getPosition();
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
    GlMaterial glMaterial(Shader::Default);
    glMaterial.bind();
    glm::mat4 model = entity->getTransform().worldPorjection();
    glMaterial.setMat4("model", model);
    glMaterial.setMat4("view", cameraProjection);
    glMaterial.setMat4("projection", screenProjection);  
    
    glMaterial.setVec3("viewPos", cameraPosition);
    glMaterial.setInt("numDirLights", dirLights.size());

    int idx = 0;
    for (auto dirLightEnt : dirLights) {
      std::stringstream base;
      base << "dirLights[" << idx++ << "]";
      DirectionalLight* light = dirLightEnt->get<DirectionalLight>();
      Vector cameraEulers = glm::eulerAngles(dirLightEnt->getTransform().getRotation());
      glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
      glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
      glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
      glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
    }
    
    glMaterial.setInt("numPointLights", pointLights.size());
    idx = 0;
    for (auto lightEnt : pointLights) {
      std::stringstream base;
      base << "pointLights[" << idx++ << "]";
      PointLight* light = lightEnt->get<PointLight>();
      glMaterial.setVec3(base.str() + ".position", lightEnt->getTransform().getPosition());
      glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
      glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
      glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
      glMaterial.setFloat(base.str() + ".constant", light->getConstant());
      glMaterial.setFloat(base.str() + ".linear", light->getLinear());
      glMaterial.setFloat(base.str() + ".quadratic", light->getQuadratic());
    }

    glMaterial.setInt("numSpotLights", spotlights.size());
    idx = 0;
    for (auto lightEnt : spotlights) {
      std::stringstream base;
      base << "spotLights[" << idx++ << "]";
      Spotlight* light = lightEnt->get<Spotlight>();
      glMaterial.setVec3(base.str() + ".position", lightEnt->getTransform().getPosition());
      Vector cameraEulers = glm::eulerAngles(lightEnt->getTransform().getRotation());
      glMaterial.setVec3(base.str() + ".direction", directionFromEulers(cameraEulers));
      glMaterial.setVec3(base.str() + ".ambient", vec3From(light->getAmbient()));
      glMaterial.setVec3(base.str() + ".diffuse", vec3From(light->getDiffuse()));
      glMaterial.setVec3(base.str() + ".specular", vec3From(light->getSpecular()));
      glMaterial.setFloat(base.str() + ".constant", light->getConstant());
      glMaterial.setFloat(base.str() + ".linear", light->getLinear());
      glMaterial.setFloat(base.str() + ".quadratic", light->getQuadratic());
      glMaterial.setFloat(base.str() + ".cutOff", glm::cos(glm::radians(light->getCutOff())));
      glMaterial.setFloat(base.str() + ".outerCutoff", glm::cos(glm::radians(light->getOuterCutOff())));
    }
    GlTexture* glTexture = GlTexture::Default;
    Material* material = entity->get<Material>();
    if (material != nullptr) {
      Texture* texture = material->getTexture();
      if (texture != nullptr) {
        glTexture = textureCache[texture->getId()];
        if (glTexture == nullptr) {
          glTexture = new GlTexture(texture);
          textureCache[texture->getId()] = glTexture;
          glTexture->load(fileSystem);
        }
      }
    }

    glTexture->bind();
    glMaterial.setInt("material.diffuse", 0);
    glMaterial.setFloat("material.shininess", 32.0f);
    draw(mesh, &glMaterial);
    glTexture->unbind();
  }

  // void Renderer::draw(Entity* camera, Entity* entity) {  
  //   Transform* cameraTransform = camera->getTransform();
  //   glm::vec3 cameraPosition = vec3From(cameraTransform->getPosition());  
  //   draw(cameraPosition, viewProjectionFrom(camera), entity);
  // }

  void Renderer::draw(Mesh* mesh, GlMaterial* material) {
    VertexArray* vao = vaoCache[mesh->getId()];
    if (vao == nullptr) {
      vao = new VertexArray();
      vao->bind();  
      
      // Create Verts Buff    
      VertexBuffer* verts = new VertexBuffer(mesh->getVertices().data(), mesh->getVertices().size() * 3 * sizeof(float));
      verts->getLayout()->pushFloat(3);
      vao->addBuffer(verts);
      // Create Norms Buff
      VertexBuffer* norms = new VertexBuffer(mesh->getNormals().data(), mesh->getNormals().size() * 3 * sizeof(float));
      norms->getLayout()->pushFloat(3);
      vao->addBuffer(norms);

      VertexBuffer* uvs = new VertexBuffer(mesh->getUvs().data(), mesh->getUvs().size() * 3 * sizeof(float));
      uvs->getLayout()->pushFloat(2);
      vao->addBuffer(uvs);  
      
      IndexBuffer* indices = new IndexBuffer((const unsigned int*)mesh->getIndices().data(), mesh->getIndices().size());
      indices->bind();
      
      vaoCache[mesh->getId()] = vao;
    } else {
      // printf("Cached BRO!\n");
      vao->bind();
    }
    
    material->bind();

    GL_FAIL_CHECK(glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0));
    
    material->unbind();
    vao->unbind();
  }

  void Renderer::stop(SystemContext* context) {
  }

  void Renderer::deinit(SystemContext* context) {
  }

}