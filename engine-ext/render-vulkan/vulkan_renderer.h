#pragma once

#include <optional>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "core/logger.h"

#include "render/renderer.h"
#include "render-vulkan/core/common.h"
#include "render-vulkan/core/instance.h"
#include "render-vulkan/core/swap_chain.h"
#include "render-vulkan/core/device.h"
#include "render-vulkan/core/surface.h"
#include "render-vulkan/core/pipeline.h"
#include "render-vulkan/core/command_pool.h"
#include "render-vulkan/render_context.h"
#include "render-vulkan/model/renderable.h"
#include "render-vulkan/model/mesh_manager.h"
#include "render-vulkan/model/texture_manager.h"
#include "render-vulkan/text/renderable.h"
#include "render-vulkan/text/font/manager.h"
#include "render-vulkan/scene.h"
#include "render-vulkan/core/render_pass.h"
#include "render-vulkan/core/context.h"
#include "render-vulkan/model/pipeline.h"
#include "render-vulkan/model/wireframe/pipeline.h"
#include "render-vulkan/text/pipeline.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace wage::render::vulkan {

  class VulkanRenderer : public Renderer {
  public:
    VulkanRenderer();

    void start();

    void stop();

    void beginRender(RenderContext* renderContext);

    void endRender(RenderContext* renderContext);

    void renderMesh(math::Transform transform, MeshSpec mesh, MaterialSpec material);

    void renderWireframe(math::Transform transform, MeshSpec mesh);

    void renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color);

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

  protected:
    RenderContext* createContext(ecs::Entity cameraEntity, Camera* camera);

    void beginMeshRender(RenderContext* renderContext);

    void endMeshRender(RenderContext* renderContext);

    void beginWireframeRender(RenderContext* renderContext);

    void endWireframeRender(RenderContext* renderContext);

    void beginUiRender(RenderContext* renderContext);

    void endUiRender(RenderContext* renderContext);

  private:
    void createSyncObjects();

    void destroySyncObjects();

    void recreateSwapChain();

    void cleanupSwapChain();

    VulkanContext context;

    ModelPipeline modelPipeline;
    
    WireframePipeline wireframePipeline;

    TextPipeline textPipeline;

    CommandPool commandPool;

    VulkanScene scene;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
    bool framebufferResized = false;

    VulkanMeshManager meshManager;

    VulkanTextureManager textureManager;

    FontManager fontManager;
  };

}