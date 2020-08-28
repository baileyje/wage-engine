#pragma once

#include <unordered_map>
#include <mutex>

#include "core/service.h"
#include "platform/window.h"
#include "assets/manager.h"
#include "ecs/manager.h"
#include "platform/platform.h"

#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"
#include "component/camera/perspective_camera.h"
#include "component/camera/orthographic_camera.h"
#include "render/components/material.h"
#include "render/components/mesh.h"
#include "render/components/font.h"
#include "render/renderable.h"
#include "render/queue.h"

namespace wage {
  namespace render {

    class Renderer : public core::Service {

    public:
      Renderer() : Service("Renderer") {
      }

      ~Renderer() {
      }

      void start() {
        auto platform = core::Core::Instance->get<platform::Platform>();
        window = platform->window();
        assetManager = core::Core::Instance->get<assets::Manager>();
        core::Core::Instance->onRender([&](const core::Frame& frame) {
          render();
        });
      }

      void render() {
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

      virtual void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color) = 0;

      virtual void renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture) = 0;

      virtual void renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material) = 0;

      inline void awaitNextQueue() {
        locks[currentProducerQueue].unlock();
        currentProducerQueue = (currentProducerQueue + 1) % 2;
        // std::cout << "CPQ: " << currentProducerQueue << std::endl;
        locks[currentProducerQueue].lock();
        meshQueues[currentProducerQueue].clear();
        uiQueues[currentProducerQueue].clear();
      }

    protected:
      void renderMeshes(ecs::EntityManager* manager, RenderContext* renderContext) {
        meshQueues[currentConsumerQueue].cull(renderContext);
        meshQueues[currentConsumerQueue].sort(renderContext);
        meshQueues[currentConsumerQueue].render(renderContext);
        // meshQueues[currentConsumerQueue].clear();
      }

      void renderUi(ecs::EntityManager* manager, RenderContext* renderContext) {
        uiQueues[currentConsumerQueue].sort(renderContext);
        uiQueues[currentConsumerQueue].render(renderContext);
        // uiQueues[currentConsumerQueue].clear();
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

      virtual void beginRender() = 0;

      virtual void endRender() = 0;

      platform::Window* window;

      assets::Manager* assetManager;

      RenderQueue meshQueues[2];

      RenderQueue uiQueues[2];

      std::mutex locks[2];

      int currentProducerQueue = 0;

      int currentConsumerQueue = 0;
    };
  }
}