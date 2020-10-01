#pragma once

#include <unordered_map>
#include <atomic>

#include "core/service.h"
#include "platform/window.h"
#include "asset/manager.h"
#include "scene/scene.h"
#include "platform/platform.h"

#include "render/components/lighting/directional_light.h"
#include "render/components/lighting/point_light.h"
#include "render/components/lighting/spotlight.h"
#include "render/components/camera/perspective_camera.h"
#include "render/components/camera/orthographic_camera.h"
#include "render/components/material.h"
#include "render/components/mesh.h"
#include "render/components/font.h"
#include "render/renderable.h"
#include "render/queue.h"
#include "render/frame.h"
#include "util/timer.h"

namespace wage {
  namespace render {

    /**
     * The renderer service is intended to provide the game engine a means to get pixels on the screen. The render provides
     * simple methods for submitting things to render. It provides virtual methods which need to be provided by a platform 
     * supported rendering implemenation. The renderer uses triple buffering to support splitting the game thread from the 
     * render thread. The game update look needs to invoke `swapFrames` whenever it has produced a new frame to render.
     */
    class Renderer : public core::Service {

    public:
      Renderer() : Service("Renderer"), _updateFrame(new Frame()) {}

      ~Renderer() {}

      void start() {
        auto platform = core::Core::Instance->get<platform::Platform>();
        window = platform->window();
        assetManager = core::Core::Instance->get<asset::Manager>();
        core::Core::Instance->onRender([&](const core::Frame& frame) {
          render();
        });
      }

      void reset() {
        _renderFrame.store(nullptr);
        _readyFrame.store(nullptr);
        _updateFrame = new Frame();
      }

      /**
       * Render the next frame of the game to the screen. This will grab the next ready frame and render it. Once complete it will 
       * move the frame back into the ready position. The render will create a contect to render the frame that includes the current
       * camera and world information.
       */
      void render() {
        auto currentFrame = _renderFrame.load(std::memory_order_acquire);
        if (!currentFrame) {
          return;
        }
        beginRender(currentFrame->context());
        beginMeshRender(currentFrame->context());
        currentFrame->renderMeshes();
        endMeshRender(currentFrame->context());
        beginUiRender(currentFrame->context());
        currentFrame->renderUi();
        endUiRender(currentFrame->context());
        endRender(currentFrame->context());

        timer.tick();
        // std::cout << "Ftime: " << timer.averageTime() << "\n";
        delete currentFrame;
        _renderFrame.store(nullptr);
      }

      /**
       * Pure virtual method for rendering a chunk of text on the screen at a specificed position.
       */
      virtual void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color){};

      /**
       * Render a simple sprite to the screen at a specific position.
       */
      virtual void renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture){};

      /**
       * Render a mesh to the screen.
       */
      virtual void renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material){};

      /**
       * Swap the frame currently beeing updated with the ready frame for the next render cycle. This should be called
       * at the end of every game update look.
       */
      inline void swapFrames() {
        _renderFrame.store(_readyFrame);
        auto manager = &scene::Scene::current().entities();
        auto camera = cameraAndEntity(manager);
        if (std::get<1>(camera)) {
          _updateFrame->prepare(createContext(std::get<0>(camera), std::get<1>(camera)));
        }
        _readyFrame.store(_updateFrame);
        _updateFrame = new Frame();
      }

      inline double averageFrameTime() {
        return timer.averageTime();
      }

    protected:
      virtual RenderContext* createContext(ecs::Entity cameraEntity, Camera* camera) {
        return new RenderContext(cameraEntity, camera, math::Vector2(window->width(), window->height()), /*dirLights, pointLights, spotlights*/ {}, {}, {});
      }

      inline Frame* updateFrame() {
        return _updateFrame;
      }

      std::tuple<ecs::Entity, Camera*> cameraAndEntity(ecs::EntityManager* manager) {
        for (auto entity : manager->with({PerspectiveCameraComponent})) {
          return {entity, entity.get<PerspectiveCamera>(PerspectiveCameraComponent)};
        }
        for (auto entity : manager->with({OrthographicCameraComponent})) {
          return {entity, entity.get<OrthographicCamera>(OrthographicCameraComponent)};
        }
        return {ecs::Entity::Invalid, nullptr};
      }

      virtual void beginRender(RenderContext* context){};

      virtual void endRender(RenderContext* context){};

      virtual void beginMeshRender(RenderContext* context){};

      virtual void endMeshRender(RenderContext* context){};

      virtual void beginUiRender(RenderContext* context){};

      virtual void endUiRender(RenderContext* context){};

      platform::Window* window;

      asset::Manager* assetManager;

      std::atomic<Frame*> _renderFrame;
      std::atomic<Frame*> _readyFrame;
      Frame* _updateFrame;

      // For FPS calculation
      util::Timer timer;
    };

  } // namespace render
} // namespace wage
