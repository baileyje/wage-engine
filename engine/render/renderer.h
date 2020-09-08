#pragma once

#include <unordered_map>
#include <atomic>
#include <chrono>

#include "core/service.h"
#include "platform/window.h"
#include "assets/manager.h"
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

#define MAX_FPS_SAMPLES 100
typedef std::chrono::high_resolution_clock::time_point TimePoint;

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
      Renderer() : Service("Renderer"), _renderFrame(&frames[0]), _readyFrame(&frames[1]), _loadingFrame(&frames[2]) {}

      ~Renderer() {}

      void start() {
        auto platform = core::Core::Instance->get<platform::Platform>();
        window = platform->window();
        assetManager = core::Core::Instance->get<assets::Manager>();
        core::Core::Instance->onRender([&](const core::Frame& frame) {
          captureFrameTime();
          render();
        });
      }

      /**
       * Render the next frame of the game to the screen. This will grab the next ready frame and render it. Once complete it will 
       * move the frame back into the ready position. The render will create a contect to render the frame that includes the current
       * camera and world information.
       */
      void render() {
        while (stale.exchange(true))
          ;
        beginRender();
        renderFrame()->render();
        endRender();
        renderFrame()->clear();
        _renderFrame = _readyFrame.exchange(_renderFrame);
      }

      /**
       * Pure virtual method for rendering a chunk of text on the screen at a specificed position.
       */
      virtual void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color) = 0;

      /**
       * Render a simple sprite to the screen at a specific position.
       */
      virtual void renderSprite(math::Vector2 position, math::Vector2 size, component::Color color, TextureSpec texture) = 0;

      /**
       * Render a mesh to the screen.
       */
      virtual void renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material) = 0;

      /**
       * Swap the frame currently beeing updated with the ready frame for the next render cycle. This should be called
       * at the end of every game update look.
       */
      inline void swapFrames() {
        _loadingFrame = _readyFrame.exchange(_loadingFrame);
        loadingFrame()->clear();
        auto manager = &scene::Scene::current().entities();
        auto camera = cameraAndEntity(manager);
        if (std::get<1>(camera)) {
          // if (!std::get<0>(camera).valid()) {
          //   core::Logger::error("No Camera");
          //   return;
          // }
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

          _readyFrame.load()->renderContext(new RenderContext(std::get<0>(camera), std::get<1>(camera), math::Vector2(window->width(), window->height()), dirLights, pointLights, spotlights));
        }
        stale.store(false);
      }

      inline double averageFrameTime() {
        return frameAverage;
      }

    protected:
      Frame* loadingFrame() {
        return _loadingFrame.load();
      }

      Frame* renderFrame() {
        return _renderFrame.load();
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

      virtual void beginRender() = 0;

      virtual void endRender() = 0;

      void captureFrameTime() {
        auto newTime = std::chrono::high_resolution_clock::now();
        double elapsed = (std::chrono::duration_cast<std::chrono::milliseconds>(newTime - lastTime)).count();
        lastTime = newTime;
        frameSum -= frameTimes[frameIndex];
        frameSum += elapsed;
        frameTimes[frameIndex] = elapsed;
        frameIndex = (frameIndex + 1) % MAX_FPS_SAMPLES;
        frameAverage = frameSum / MAX_FPS_SAMPLES;
      }

      platform::Window* window;

      assets::Manager* assetManager;

      // Triple buffer the frames.
      Frame frames[3];
      std::atomic<Frame*> _renderFrame;
      std::atomic<Frame*> _readyFrame;
      std::atomic<Frame*> _loadingFrame;
      std::atomic<bool> stale;

      // For FPS calculation
      TimePoint lastTime;
      int frameIndex = 0;
      double frameSum = 0;
      double frameTimes[MAX_FPS_SAMPLES];
      double frameAverage;
    };
  } // namespace render
} // namespace wage
