#include "engine/render/renderer.h"

#include <vector>
#include <sstream>

#include "engine/platform/platform.h"

#include "engine/core/logger.h"
#include "engine/core/system/context.h"

#include "engine/render/context.h"

#include "engine/entity/component/camera/camera.h"

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
    // TODO: Make some kind of compomnent list type to avoid this iterator shaz bot.
    EntityManager* manager = context->get<EntityManager>();
    for (auto light = manager->componentManager().begin<DirectionalLight>(); light != manager->componentManager().end<DirectionalLight>(); ++light) {
      dirLights.push_back(light.operator->().operator->());
    }
    for (auto light = manager->componentManager().begin<PointLight>(); light != manager->componentManager().end<PointLight>(); ++light) {
      pointLights.push_back(light.operator->().operator->());
    }
    for (auto light = manager->componentManager().begin<Spotlight>(); light != manager->componentManager().end<Spotlight>(); ++light) {
      spotlights.push_back(light.operator->().operator->());
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
    if (!camera.valid()) {
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