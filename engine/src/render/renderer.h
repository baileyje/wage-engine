#pragma once

#include <unordered_map>
#include <array>
#include <atomic>
#include <mutex>
#include <condition_variable>

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
      Renderer() : Service("Renderer") /*, _renderFrame(nullptr), _readyFrame(nullptr), _updateFrame(new Frame())*/ {
        
      }

      ~Renderer() {}

      void start() {
        auto platform = core::Core::Instance->get<platform::Platform>();
        window = platform->window();
        assetManager = core::Core::Instance->get<asset::Manager>();
        core::Core::Instance->onRender([&](const core::Frame& frame) {
          if (rendering) render();
        });
      }

      void stop() {
        // std::cout << "Stopping Renderer" <<  std::endl; 
        // std::unique_lock<std::mutex> lock(renderLock);
        // renderReady = true;
        // rendering = false;
        // lock.unlock();
        // renderReadyCv.notify_all();
      }

      void reset() {
        for (auto frame : frames) {
          frame.clear();
        }
        updateFrameIdx = 0;
        renderFrameIdx = 1;
      }

      /**
       * Render the next frame of the game to the screen. This will grab the next ready frame and render it. Once complete it will 
       * move the frame back into the ready position. The render will create a contect to render the frame that includes the current
       * camera and world information.
       */
      void render() {
        std::unique_lock<std::mutex> lock(renderLock);
        renderReadyCv.wait(lock, [this](){ return renderReady; });
        renderReady = false;
        auto currentFrame = &frames[renderFrameIdx];
        if (!currentFrame || !currentFrame->context())  {
          lock.unlock();
          renderReadyCv.notify_one();
          return;
        }
        beginRender(currentFrame->context());
        beginMeshRender(currentFrame->context());
        currentFrame->renderMeshes();
        endMeshRender(currentFrame->context());
        beginWireframeRender(currentFrame->context());
        currentFrame->renderWireframes();
        endWireframeRender(currentFrame->context());
        beginUiRender(currentFrame->context());
        currentFrame->renderUi();
        endUiRender(currentFrame->context());
        endRender(currentFrame->context());

        timer.tick();
        destroyContext(currentFrame->context());
        lock.unlock();
        renderReadyCv.notify_one();
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
      virtual void renderMesh(math::Transform transform, MeshSpec mesh, MaterialSpec material) {};

      /**
       * Render wireframe.  FOR DEBUGGING.
       */
      virtual void renderWireframe(math::Transform transform, MeshSpec mesh) { };

      /**
       * Swap the frame currently beeing updated with the ready frame for the next render cycle. This should be called
       * at the end of every game update look.
       */
      inline void swapFrames() {
        std::unique_lock<std::mutex> lock(renderLock);
        renderReadyCv.wait(lock, [this](){ return !renderReady; });
        updateFrameIdx = (updateFrameIdx + 1) % frames.size();
        renderFrameIdx = (renderFrameIdx + 1) % frames.size();
        auto manager = &scene::Scene::current().entities();
        auto camera = cameraAndEntity(manager);
        if (std::get<1>(camera)) {
          frames[renderFrameIdx].prepare(createContext(std::get<0>(camera), std::get<1>(camera)));
        }
        frames[updateFrameIdx].clear();        
        renderReady = true;
        lock.unlock();
        renderReadyCv.notify_one();
      }

      inline double averageFrameTime() {
        return timer.averageTime();
      }

      bool renderWireFrames = false;

    protected:
      virtual RenderContext* createContext(ecs::Entity cameraEntity, Camera* camera) = 0;

      virtual void destroyContext(RenderContext* context) {
        if (context) delete context;
      } 

      inline Frame* updateFrame() {
        return &frames[updateFrameIdx];
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

      virtual void beginWireframeRender(RenderContext* context){};

      virtual void endWireframeRender(RenderContext* context){};

      virtual void beginUiRender(RenderContext* context){};

      virtual void endUiRender(RenderContext* context){};

      platform::Window* window;

      asset::Manager* assetManager;

      std::array<Frame, 2> frames;

      volatile int updateFrameIdx = 0;
      volatile int renderFrameIdx = 1;

      std::mutex renderLock;
      std::condition_variable renderReadyCv;
      bool renderReady;

      // For FPS calculation
      util::Timer timer;
      
    private:
      volatile bool rendering = true;
    };

  } // namespace render
} // namespace wage
