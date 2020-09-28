#pragma once

#include <optional>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "core/logger.h"

#include "render/renderer.h"
#include "render/mesh/manager.h"
#include "render-vulkan/common.h"
#include "render-vulkan/instance.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/device.h"
#include "render-vulkan/surface.h"
#include "render-vulkan/model_pipeline.h"
#include "render-vulkan/command_pool.h"
#include "render-vulkan/render_context.h"
#include "render-vulkan/model_renderable.h"
#include "render-vulkan/model_manager.h"
#include "render-vulkan/scene.h"
#include "render-vulkan/ubo_scene.h"
#include "render-vulkan/render_pass.h" 
#include "render-vulkan/context.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace wage {
  namespace render {

    class VulkanRenderer : public Renderer {
    public:
      VulkanRenderer();

      void start();

      void stop();

      void beginRender(RenderContext* renderContext);

      void endRender(RenderContext* renderContext);

      void renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material);

      static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
      
    protected:
      RenderContext* createContext(ecs::Entity cameraEntity, Camera* camera);

    private:

      void createSyncObjects();

      void destroySyncObjects();

      void recreateSwapChain();

      void cleanupSwapChain();

      VulkanContext context;

      ModelPipeline pipeline;
      
      CommandPool commandPool;

      VulkanScene scene;

      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
      bool framebufferResized = false;

      MeshManager meshManager;
      ModelManager modelManager;
    };

  }
}