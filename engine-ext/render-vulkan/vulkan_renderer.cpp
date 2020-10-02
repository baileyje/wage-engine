#include "render-vulkan/vulkan_renderer.h"

#include "render-vulkan/ubo_scene.h"
#include "render-vulkan/ui/ubo.h"

namespace wage::render::vulkan {

  VulkanRenderer::VulkanRenderer() : modelPipeline(&context), textPipeline(&context), commandPool(&context) {}

  void VulkanRenderer::start() {
    Renderer::start();
    meshManager.assetManager(assetManager);
    meshManager.generatePrimitives();
    fontManager.assetManager = assetManager;
    if (glfwVulkanSupported()) {
      core::Logger::info("Vulkan Supported FTW");
    } else {
      core::Logger::error("Vulkan not supported :(");
      exit(99);
    }
    // TODO: Integrate window resize events into the platform window handling...
    // auto glfwWindow = window->as<GLFWwindow>();
    // glfwSetWindowUserPointer(glfwWindow, this);
    // glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);

    context.create(window);
    modelPipeline.create();
    textPipeline.create();
    commandPool.create();
    createSyncObjects();
    scene.create(&context.device, &commandPool, &modelPipeline, &textPipeline, context.frameCount);
  }

  void VulkanRenderer::stop() {
    vkDeviceWaitIdle(context.device.logical);
    fontManager.destroy(&context.device);
    modelManager.destroy(&context.device);
    scene.destroy(&context.device);
    commandPool.cleanupBuffers();
    modelPipeline.cleanup();
    modelPipeline.cleanup();
    destroySyncObjects();
    commandPool.destroy();
    context.destroy();
  }

  void VulkanRenderer::beginRender(RenderContext* renderContext) {
    vkWaitForFences(context.device.logical, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(context.device.logical, context.swapChain.wrapped, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image!");
    }
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
      vkWaitForFences(context.device.logical, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];
    auto commandBuffer = commandPool.beginCommandBuffer(imageIndex);
    auto vkContext = static_cast<VulkanRenderContext*>(renderContext);
    vkContext->device = &context.device;
    vkContext->commandPool = &commandPool;
    vkContext->modelPipeline = &modelPipeline;
    vkContext->textPipeline = &textPipeline;
    vkContext->imageIndex = imageIndex;
    vkContext->imageCount = context.frameCount;
    vkContext->commandBuffer = commandBuffer;

    context.renderPass.begin(commandBuffer, imageIndex);
  }

  void VulkanRenderer::beginMeshRender(RenderContext* renderContext) {
    auto vkContext = static_cast<VulkanRenderContext*>(renderContext);
    auto commandBuffer = vkContext->commandBuffer;
    modelPipeline.bind(commandBuffer);

    UniformBufferScene ubo{};
    ubo.view = renderContext->viewProjection().glm();
    ubo.proj = renderContext->screenProjection().glm();
    ubo.proj[1][1] *= -1;
    scene.modelUniformBuffers[vkContext->imageIndex].fillWith(&ubo, sizeof(ubo));
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, modelPipeline.layout, 0, 1, &scene.modelDescriptorSets[vkContext->imageIndex], 0, nullptr);
  }

  void VulkanRenderer::endMeshRender(RenderContext* renderContext) {
    auto vkContext = static_cast<VulkanRenderContext*>(renderContext);
  }

  void VulkanRenderer::beginUiRender(RenderContext* renderContext) {
    auto vkContext = static_cast<VulkanRenderContext*>(renderContext);
    auto commandBuffer = vkContext->commandBuffer;
    textPipeline.bind(commandBuffer);

    UiUniformBuffer ubo{};
    auto projection = math::Matrix::orthographic(0.0f, renderContext->screenSize().x, 0.0f, renderContext->screenSize().y);
    ubo.proj = projection.glm();

    scene.uiUniformBuffers[vkContext->imageIndex].fillWith(&ubo, sizeof(ubo));
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, textPipeline.layout, 0, 1, &scene.uiDescriptorSets[vkContext->imageIndex], 0, nullptr);
  }

  void VulkanRenderer::endUiRender(RenderContext* renderContext) {
  }

  void VulkanRenderer::endRender(RenderContext* renderContext) {
    auto vkContext = static_cast<VulkanRenderContext*>(renderContext);
    context.renderPass.end(vkContext->commandBuffer);
    commandPool.endCommandBuffer(vkContext->commandBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkContext->commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(context.device.logical, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(context.device.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
      throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {context.swapChain.wrapped};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &vkContext->imageIndex;

    VkResult result = vkQueuePresentKHR(context.device.presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
      framebufferResized = false;
      recreateSwapChain();
    } else if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image!");
    }
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    for (auto& cleanup : vkContext->cleanups) {
      cleanup(vkContext);
    }
  }

  void VulkanRenderer::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    std::cout << "Resize!!!\n";
    // auto renderer = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
    // renderer->framebufferResized = true;
  }

  void VulkanRenderer::renderMesh(math::Transform transform, MeshSpec* mesh, MaterialSpec* material) {
    updateFrame()->meshQueue().add<ModelRenderable>(assetManager, &meshManager, &modelManager, transform, mesh, material);
  }

  void VulkanRenderer::renderText(math::Vector2 position, std::string text, FontSpec font, component::Color color) {
    updateFrame()->uiQueue().add<TextRenderable>(&fontManager, position, text, font, color);
  }

  RenderContext* VulkanRenderer::createContext(ecs::Entity cameraEntity, Camera* camera) {
    return new VulkanRenderContext(cameraEntity, camera, math::Vector2(window->width(), window->height()), /*dirLights, pointLights, spotlights*/ {}, {}, {});
  }

  void VulkanRenderer::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(context.frameCount, VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      if (vkCreateSemaphore(context.device.logical, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
          vkCreateSemaphore(context.device.logical, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
          vkCreateFence(context.device.logical, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to create synchronization objects for a frame!");
      }
    }
  }

  void VulkanRenderer::destroySyncObjects() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
      vkDestroySemaphore(context.device.logical, renderFinishedSemaphores[i], nullptr);
      vkDestroySemaphore(context.device.logical, imageAvailableSemaphores[i], nullptr);
      vkDestroyFence(context.device.logical, inFlightFences[i], nullptr);
    }
  }

  void VulkanRenderer::recreateSwapChain() {
    int width = 0, height = 0;
    auto glfwWindow = window->as<GLFWwindow>();
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    while (width == 0 || height == 0) {
      glfwGetFramebufferSize(glfwWindow, &width, &height);
      glfwWaitEvents();
    }
    vkDeviceWaitIdle(context.device.logical);
    cleanupSwapChain();
    context.swapChain.create(window->width(), window->height());
    modelPipeline.create();
    textPipeline.create();
    context.swapChain.createDepthResources();
    context.swapChain.createFrameBuffers();
    commandPool.create();
  }

  void VulkanRenderer::cleanupSwapChain() {
    commandPool.cleanupBuffers();
    modelPipeline.cleanup();
    textPipeline.cleanup();
    context.swapChain.cleanup();
  }
}