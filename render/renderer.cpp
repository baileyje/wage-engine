#include "render/renderer.h"

#include <vector>
#include <sstream>

#include "platform/platform.h"

#include "core/logger.h"
#include "core/system/context.h"

#include "render/context.h"

#include "entity/component/camera/camera.h"

namespace wage {

  Renderer::~Renderer() {
  }

  void Renderer::init(SystemContext* context)  {
  }

  void Renderer::start(SystemContext* context) {
    Platform* platform = context->get<Platform>();
    window = platform->getWindow();
    assetManager = context->get<AssetManager>();
    // // TODO: Listen for changes to entities and figure out how to get these!!!
    // // ALSO, GROSS
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
  }

  void Renderer::renderMeshes(EntityManager* manager, RenderContext* renderContext) {     
    meshQueue.cull(renderContext);
    meshQueue.sort(renderContext);
    meshQueue.render(renderContext);
    meshQueue.clear();
  }

  void Renderer::renderUi(EntityManager* manager, RenderContext* renderContext) { 
    uiQueue.cull(renderContext);
    uiQueue.sort(renderContext);
    uiQueue.render(renderContext);
    uiQueue.clear();
  }

  void Renderer::update(SystemContext* context) {  
    beginUpdate();
    EntityManager* manager = context->get<EntityManager>();
    auto camera = Camera::main;
    if (camera == nullptr) {
      Logger::error("No Camera");
      return;
    }
    RenderContext renderContext(camera, Vector2(window->width(),  window->height()),
      dirLights, pointLights, spotlights);
    
    renderMeshes(manager, &renderContext);
    // TODO: Sprites
    renderUi(manager, &renderContext);
    endUpdate();
  }

  void Renderer::stop(SystemContext* context) {
  }

  void Renderer::deinit(SystemContext* context) {
  }

}