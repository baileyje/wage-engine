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
#include "render/context.h"
#include "render/mesh_renderable.h"
#include "render/queue.h"

#include "entity/component/render/mesh.h"
#include "entity/component/render/material.h"
#include "entity/component/camera/camera.h"

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

    textureManager.setFileSystem(fileSystem);
    
    // TODO: Listen for changes to entities and figure out how to get these!!!
    // ALSO, GROSS
    EntityManager* manager = context->get<EntityManager>();
    auto dirLightEntities = manager->with("DirectionalLight");
    for (auto entity : dirLightEntities) {
      dirLights.push_back(entity->get<DirectionalLight>());
    }
    auto pointLightEntities = manager->with("PointLight");
    for (auto entity : pointLightEntities) {
      pointLights.push_back(entity->get<PointLight>());
    }
    auto spotlightEntities = manager->with("Spotlight");
    for (auto entity : spotlightEntities) {
      spotlights.push_back(entity->get<Spotlight>());
    }
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
  }

  void renderMeshes(EntityManager* manager, RenderContext* renderContext) {
    RenderQueue queue;
    for (auto entity : manager->with("Mesh")) {
      MeshRenderable* renderable = new MeshRenderable(entity->getTransform().getPosition(), entity->getTransform().getScale(), 
      entity->get<Mesh>(), entity->getTransform().worldPorjection(), entity->get<Material>());      
      queue.add(renderable);
    }    
    queue.cull(renderContext);
    queue.sort(renderContext);
    queue.render(renderContext);
  }

  void Renderer::update(SystemContext* context) {  
    GL_FAIL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
    GL_FAIL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    EntityManager* manager = context->get<EntityManager>();
    auto camera = Camera::main;
    if (camera == nullptr) {
      Logger::error("No Camera");
      return;
    }
    Transform cameraTransform = *camera->getTransform();    
    RenderContext renderContext(&vaoManager, &textureManager, camera, Vector2(screenWidth, screenHeight),
      dirLights, pointLights, spotlights);
    
    renderMeshes(manager, &renderContext);
    // TODO: Sprites
    // TODO: UI panes

    GL_FAIL_CHECK(glfwSwapBuffers(window));
    GL_FAIL_CHECK(glfwPollEvents());
  }

  void Renderer::stop(SystemContext* context) {
  }

  void Renderer::deinit(SystemContext* context) {
  }

}