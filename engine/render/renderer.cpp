#include "render/renderer.h"

#include <vector>
#include <sstream>
#include <tuple>

#include "platform/platform.h"
#include "core/logger.h"
#include "core/system/context.h"
#include "render/context.h"
#include "entity/component/camera/perspective_camera.h"
#include "entity/component/camera/orthographic_camera.h"
#include "render/material.h"

namespace wage {

  Renderer::~Renderer() {
  }

  void Renderer::init(SystemContext* context)  {
  }

  void Renderer::start(SystemContext* context) {
    Platform* platform = context->get<Platform>();
    window = platform->window();
    assetManager = context->get<AssetManager>();
  }

  void Renderer::renderMeshes(EntityManager* manager, RenderContext* renderContext) {    
    for (auto entity : manager->registry()->with<Mesh>()) {
      auto trans = entity.get<Transform>();
      auto mesh = entity.get<Mesh>();
      auto mat = entity.get<Material>();
      renderMesh(trans, mesh, mat);
    }
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

  std::tuple<Entity, Camera*> cameraAndEntity(EntityManager* manager) {
    for (auto entity : manager->registry()->with<PerspectiveCamera>())  {
      return { entity, entity.get<PerspectiveCamera>().get() };
    }
    for (auto entity : manager->registry()->with<OrthographicCamera>())  {
      return { entity, entity.get<OrthographicCamera>().get() };
    }
    return { Entity(), nullptr };
  }

  void Renderer::update(SystemContext* context) {  
    beginUpdate();
    auto manager = context->get<EntityManager>();
    auto camera = cameraAndEntity(manager);
    if (!std::get<0>(camera).valid()) {
      Logger::error("No Camera");
      return;
    }
    std::vector<Entity> dirLights;
    for (auto ent : manager->registry()->with<DirectionalLight>()) {
      dirLights.push_back(ent);
    }
    std::vector<Entity> pointLights;
    for (auto ent : manager->registry()->with<PointLight>()) {
      pointLights.push_back(ent);
    }
    std::vector<Entity> spotlights;
    for (auto ent : manager->registry()->with<Spotlight>()) {
      spotlights.push_back(ent);
    }
    RenderContext renderContext(std::get<0>(camera), std::get<1>(camera), Vector2(window->width(),  window->height()), dirLights, pointLights, spotlights);
  
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