#pragma once

#pragma < filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "engine.h"
#include "render-vulkan/vulkan_renderer.h"
#include "gui/widgets/model.h"
#include "gui/windows/asset_browser.h"
#include "gui/windows/entity_browser.h"

static void checkVkResult(VkResult err) {
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0)
    abort();
}

namespace wage::editor {

  class Gui : public core::Service {
  public:
    Gui(std::filesystem::path resourceBase) : Service("ImGui"), meshBrowser("Mesh", resourceBase / "mesh"), textureBrowser("Texture", resourceBase / "texture"), entityBrowser(&meshBrowser, &textureBrowser) {}

    void start() {
      renderer = static_cast<render::vulkan::VulkanRenderer*>(core::Core::Instance->get<render::Renderer>());
      auto platform = core::Core::Instance->get<platform::Platform>();
      auto window = platform->window();
      auto glfwWindow = window->as<GLFWwindow>();

      setupVulkanStuff();

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO();
      (void)io;

      ImGui::StyleColorsDark();

      ImGui_ImplGlfw_InitForVulkan(glfwWindow, true);
      ImGui_ImplVulkan_InitInfo init_info = {};
      init_info.Instance = renderer->context.instance.wrapped;
      init_info.PhysicalDevice = renderer->context.device.physical;
      init_info.Device = renderer->context.device.logical;
      init_info.QueueFamily = queueFamily;
      init_info.Queue = renderer->context.device.graphicsQueue;
      init_info.PipelineCache = nullptr;
      init_info.DescriptorPool = descriptorPool;
      init_info.Allocator = nullptr;
      init_info.MinImageCount = renderer->context.swapChain.images.size();
      init_info.ImageCount = renderer->context.swapChain.images.size();
      init_info.CheckVkResultFn = checkVkResult;
      ImGui_ImplVulkan_Init(&init_info, renderer->context.renderPass.wrapped);

      uploadFonts();

      renderer->addTask([&](render::vulkan::VulkanRenderContext* context) {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        buildUi();

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, context->commandBuffer);
      });
    }

    void buildUi() {
      if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);

      entityBrowser.apply();
      meshBrowser.apply();
      textureBrowser.apply();
    }

  private:
    void setupVulkanStuff() {
      VkResult err;
      {
        VkDescriptorPoolSize pool_sizes[] = {
            {VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(renderer->context.device.logical, &pool_info, nullptr, &descriptorPool);
        checkVkResult(err);
      }

      {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(renderer->context.device.physical, &count, NULL);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(renderer->context.device.physical, &count, queues);
        for (uint32_t i = 0; i < count; i++)
          if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamily = i;
            break;
          }
        free(queues);
        IM_ASSERT(queueFamily != (uint32_t)-1);
      }
    }

    void uploadFonts() {
      VkCommandBuffer commandBuffer = renderer->commandPool.beginSingleTimeCommands();
      ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
      renderer->commandPool.endSingleTimeCommands(renderer->context.device.graphicsQueue, commandBuffer);
      ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    std::filesystem::path resourceBase;

    render::vulkan::VulkanRenderer* renderer;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    uint32_t queueFamily = (uint32_t)-1;

    AssetBrowser meshBrowser;

    AssetBrowser textureBrowser;

    EntityBrowser entityBrowser;

    bool showDemoWindow = false;
  };
}