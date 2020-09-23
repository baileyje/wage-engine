#pragma once

#include <optional>
#include <set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "core/logger.h"

#include "render/renderer.h"
#include "render-vulkan/common.h"
#include "render-vulkan/instance.h"
#include "render-vulkan/swap_chain.h"
#include "render-vulkan/device.h"
#include "render-vulkan/graphics_pipeline.h"
#include "render-vulkan/command_pool.h"

// TODO: Move to engine math
#include <glm/glm.hpp>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace wage {
  namespace render {

    class VulkanRenderer : public Renderer {
    public:
      VulkanRenderer() : surface(&instance), swapChain(&device), pipeline(&device, &swapChain), commandPool(&device, &swapChain) {}

      void start() {
        Renderer::start();
        if (glfwVulkanSupported()) {
          core::Logger::info("Vulkan Supported FTW");
        } else {
          core::Logger::error("Vulkan not supported :(");
          exit(99);
        }
        auto glfwWindow = window->as<GLFWwindow>();
        // TODO: Integrate window resize events into the platform window handling...
        // glfwSetWindowUserPointer(glfwWindow, this);
        // glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);

        instance.create(enableValidationLayers);
        surface.create(glfwWindow);
        device.create(instance.wrapped(), surface);
        swapChain.create(window, surface);
        pipeline.create();
        swapChain.createDepthResources(); 
        swapChain.createFrameBuffers(pipeline.renderPass());
        commandPool.create(surface, pipeline);
        createSyncObjects();
      }

      void stop() {
        commandPool.cleanup();
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
          vkDestroySemaphore(device.logical(), renderFinishedSemaphores[i], nullptr);
          vkDestroySemaphore(device.logical(), imageAvailableSemaphores[i], nullptr);
          vkDestroyFence(device.logical(), inFlightFences[i], nullptr);
        }
        swapChain.cleanupFrameBuffers();
        pipeline.cleanup();
        swapChain.cleanup();
        surface.destroy();
        instance.destroy();
      }

      void beginRender() {
        vkWaitForFences(device.logical(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.logical(), swapChain.wrapped(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
          recreateSwapChain();
          return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
          throw std::runtime_error("failed to acquire swap chain image!");
        }

        updateUniformBuffer(imageIndex);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
          vkWaitForFences(device.logical(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandPool.commandBuffers()[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(device.logical(), 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(device.graphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
          throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain.wrapped()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(device.presentQueue(), &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
          framebufferResized = false;
          recreateSwapChain();
        } else if (result != VK_SUCCESS) {
          throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
      }

      static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto renderer = reinterpret_cast<VulkanRenderer*>(glfwGetWindowUserPointer(window));
        renderer->framebufferResized = true;
      }

    private:

      void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapChain.images().size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
          if (vkCreateSemaphore(device.logical(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
              vkCreateSemaphore(device.logical(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
              vkCreateFence(device.logical(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
          }
        }
      }

      void recreateSwapChain() {
        int width = 0, height = 0;
        auto glfwWindow = window->as<GLFWwindow>();
        glfwGetFramebufferSize(glfwWindow, &width, &height);
        while (width == 0 || height == 0) {
          glfwGetFramebufferSize(glfwWindow, &width, &height);
          glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.logical());
        cleanupSwapChain();
        swapChain.create(window, surface);
        pipeline.create();
        swapChain.createDepthResources();
        swapChain.createFrameBuffers(pipeline.renderPass());
        commandPool.create(surface, pipeline);
      }

      void cleanupSwapChain() {
        swapChain.cleanupFrameBuffers();
        commandPool.cleanupBuffers();
        pipeline.cleanup();
        swapChain.cleanup();
        vkDestroySwapchainKHR(device.logical(), swapChain.wrapped(), nullptr);
      }

      void updateUniformBuffer(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.extent().width / (float)swapChain.extent().height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;
        
        UniformBufferObject ubo2{};
        ubo2.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo2.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo2.proj = glm::perspective(glm::radians(45.0f), swapChain.extent().width / (float)swapChain.extent().height, 0.1f, 10.0f);
        ubo2.proj[1][1] *= -1;

         commandPool.model1.uniformBuffers[currentImage].fillWith(&ubo, sizeof(ubo));
         commandPool.model2.uniformBuffers[currentImage].fillWith(&ubo2, sizeof(ubo));
      }

      Instance instance;
      Surface surface;
      Device device;
      SwapChain swapChain;

      GraphicsPipeline pipeline;
      CommandPool commandPool;

      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
      bool framebufferResized = false;
    };

  }
}