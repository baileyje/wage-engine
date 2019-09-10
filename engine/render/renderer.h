#pragma once

#include <unordered_map>
#include <mutex>

#include "core/service.h"
#include "platform/window.h"
#include "assets/manager.h"
#include "ecs/entity_manager.h"

#include "component/lighting/directional_light.h"
#include "component/lighting/point_light.h"
#include "component/lighting/spotlight.h"
#include "render/material.h"
#include "render/mesh.h"
#include "render/font.h"
#include "render/renderable.h"
#include "render/queue.h"

namespace wage { namespace render {

  class Renderer : public core::Service {

  public:
    Renderer() : Service("Renderer") {
    }

    virtual ~Renderer();

    virtual void start();

    virtual void render();

    virtual void renderText(math::Vector2 position, std::string text, Font font, math::Color color) = 0;

    virtual void renderSprite(math::Vector2 position, math::Vector2 size, math::Color color, Texture texture) = 0;

    virtual void renderMesh(math::Transform transform, Mesh* mesh, Material* material) = 0;

    void awaitNextQueue();

  protected:

    void renderMeshes(ecs::EntityManager* manager, RenderContext* renderContext);

    void renderUi(ecs::EntityManager* manager, RenderContext* renderContext);

    virtual void beginRender() = 0;

    virtual void endRender() = 0;

  protected:

    platform::Window* window;

    assets::Manager* assetManager;

    RenderQueue meshQueues[2];

    RenderQueue uiQueues[2];

    std::mutex locks[2];

    int currentProducerQueue = 0;

    int currentConsumerQueue = 0;
  };
} }