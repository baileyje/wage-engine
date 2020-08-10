#include "render/renderer.h"

#include <vector>
#include <sstream>
#include <tuple>

#include "platform/platform.h"
#include "core/logger.h"
#include "ecs/system_context.h"
#include "render/context.h"
#include "component/camera/perspective_camera.h"
#include "component/camera/orthographic_camera.h"
#include "render/material.h"

namespace wage {
  namespace render {

    Renderer::~Renderer() {
    }

    void Renderer::start() {
      auto platform = core::Core::Instance->get<platform::Platform>();
      window = platform->window();
      assetManager = core::Core::Instance->get<assets::Manager>();
      core::Core::Instance->onRender([&](const core::Frame& frame) {
        render();
      });
    }

    void Renderer::renderMeshes(ecs::EntityManager* manager, RenderContext* renderContext) {
      meshQueues[currentConsumerQueue].cull(renderContext);
      meshQueues[currentConsumerQueue].sort(renderContext);
      meshQueues[currentConsumerQueue].render(renderContext);
      meshQueues[currentConsumerQueue].clear();
    }

    void Renderer::renderUi(ecs::EntityManager* manager, RenderContext* renderContext) {
      uiQueues[currentConsumerQueue].sort(renderContext);
      uiQueues[currentConsumerQueue].render(renderContext);
      uiQueues[currentConsumerQueue].clear();
    }

    std::tuple<ecs::Entity, component::Camera*> cameraAndEntity(ecs::EntityManager* manager) {
      for (auto entity : manager->with({PerspectiveCameraComponent})) {
        return {entity, entity.get<component::PerspectiveCamera>(PerspectiveCameraComponent)};
      }
      for (auto entity : manager->with({OrthographicCameraComponent})) {
        return {entity, entity.get<component::OrthographicCamera>(OrthographicCameraComponent)};
      }
      return {ecs::Entity::Invalid, nullptr};
    }

    void Renderer::render() {
      beginRender();
      locks[currentConsumerQueue].lock();
      auto manager = core::Core::Instance->get<ecs::EntityManager>();
      auto camera = cameraAndEntity(manager);
      if (!std::get<0>(camera).valid()) {
        core::Logger::error("No Camera");
        return;
      }
      std::vector<ecs::Entity> dirLights;
      // for (auto ent : manager->with<DirectionalLight>()) {
      //   dirLights.push_back(ent);
      // }
      std::vector<ecs::Entity> pointLights;
      // for (auto ent : manager->with<PointLight>()) {
      //   pointLights.push_back(ent);
      // }
      std::vector<ecs::Entity> spotlights;
      // for (auto ent : manager->with<Spotlight>()) {
      //   spotlights.push_back(ent);
      // }
      RenderContext renderContext(std::get<0>(camera), std::get<1>(camera), math::Vector2(window->width(), window->height()), dirLights, pointLights, spotlights);

      renderMeshes(manager, &renderContext);
      // TODO: Sprites
      renderUi(manager, &renderContext);
      endRender();
      locks[currentConsumerQueue].unlock();
      currentConsumerQueue = (currentConsumerQueue + 1) % 2;
      // std::cout << "CCQ: " << currentConsumerQueue << std::endl;
    }

    void Renderer::awaitNextQueue() {
      locks[currentProducerQueue].unlock();
      currentProducerQueue = (currentProducerQueue + 1) % 2;
      // std::cout << "CPQ: " << currentProducerQueue << std::endl;
      locks[currentProducerQueue].lock();
    }

  }
}