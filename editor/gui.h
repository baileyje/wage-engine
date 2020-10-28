#pragma once

#include <filesystem>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "engine.h"
#include "render-vulkan/vulkan_renderer.h"
#include "gui/widgets/model.h"
#include "gui/windows/asset_browser.h"
#include "gui/windows/entity_browser.h"
#include "gui/windows/scene_browser.h"
#include "gui/windows/scene_save.h"

static void checkVkResult(VkResult err) {
  if (err == 0)
    return;
  fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
  if (err < 0)
    abort();
}

namespace wage::editor {

  class Gui : public core::Service, public messaging::MessageListener<input::KeyEvent> {
  public:
    EntityBrowser entityBrowser;

    Gui(std::filesystem::path resourceBase) : Service("ImGui"), meshBrowser("Mesh", resourceBase / "mesh"), textureBrowser("Texture", resourceBase / "texture"), entityBrowser(&meshBrowser, &textureBrowser), sceneBrowser(resourceBase / ".." / "scenes"), sceneSave(resourceBase / ".." / "scenes") {}

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

      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
      //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
      io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
      // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
      io.ConfigDockingWithShift = true;

      ImGui::StyleColorsDark();
      // setupStyle();

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

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        buildUi();

        ImGui::Render();
        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, context->commandBuffer);
      });

      sceneBrowser.onSelect = [&](std::string path) {
        std::cout << "Open Scene: " << path << "\n";
        memory::Allocator::Permanent()->clear();
        // core::Core::Instance->get<physics::Physics>()->reset();
        core::Core::Instance->get<scene::Manager>()->reset();
        auto serializer = core::Core::Instance->get<serialize::SceneSerializer>();
        serializer->load({{"scenes", path}}, scene::Scene::current());
      };

      core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(this);
    }

    void buildUi() {
      if (showDemoWindow) ImGui::ShowDemoWindow(&showDemoWindow);
      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("New", "Cmd+N")) {
            newScene();
          }
          if (ImGui::MenuItem("Open", "Cmd+O")) {
            sceneBrowser.show();
          }
          if (ImGui::MenuItem("Save", "Cmd+S")) {
            sceneSave.show();
          }
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }

      entityBrowser.apply();
      meshBrowser.apply();
      textureBrowser.apply();
      sceneBrowser.apply();
      sceneSave.apply();
    }

    inline bool on(const input::KeyEvent& event) {
      if (event.key() == input::Key::n && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
        std::cout << "New scene...\n";
        newScene();
      }
      if (event.key() == input::Key::s && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
        std::cout << "Save scene...\n";
        sceneSave.show();
      }
      if (event.key() == input::Key::o && event.type() == input::KeyEventType::press && event.set(input::KeyModifier::super)) {
        sceneBrowser.show();
      }
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

    void setupStyle() {
      constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b) {
        return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
      };

      auto& style = ImGui::GetStyle();
      ImVec4* colors = style.Colors;

      const ImVec4 bgColor = ColorFromBytes(5, 5, 5);
      const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
      const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

      const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
      const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
      const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

      const ImVec4 textColor = ColorFromBytes(255, 255, 255);
      const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
      const ImVec4 borderColor = ColorFromBytes(44, 40, 40);

      colors[ImGuiCol_Text] = textColor;
      colors[ImGuiCol_TextDisabled] = textDisabledColor;
      colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
      colors[ImGuiCol_WindowBg] = bgColor;
      colors[ImGuiCol_ChildBg] = bgColor;
      colors[ImGuiCol_PopupBg] = bgColor;
      colors[ImGuiCol_Border] = borderColor;
      colors[ImGuiCol_BorderShadow] = borderColor;
      colors[ImGuiCol_FrameBg] = panelColor;
      colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
      colors[ImGuiCol_FrameBgActive] = panelActiveColor;
      colors[ImGuiCol_TitleBg] = bgColor;
      colors[ImGuiCol_TitleBgActive] = bgColor;
      colors[ImGuiCol_TitleBgCollapsed] = bgColor;
      colors[ImGuiCol_MenuBarBg] = panelColor;
      colors[ImGuiCol_ScrollbarBg] = panelColor;
      colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
      colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
      colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
      colors[ImGuiCol_CheckMark] = panelActiveColor;
      colors[ImGuiCol_SliderGrab] = panelHoverColor;
      colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
      colors[ImGuiCol_Button] = panelColor;
      colors[ImGuiCol_ButtonHovered] = panelHoverColor;
      colors[ImGuiCol_ButtonActive] = panelHoverColor;
      colors[ImGuiCol_Header] = panelColor;
      colors[ImGuiCol_HeaderHovered] = panelHoverColor;
      colors[ImGuiCol_HeaderActive] = panelActiveColor;
      colors[ImGuiCol_Separator] = borderColor;
      colors[ImGuiCol_SeparatorHovered] = borderColor;
      colors[ImGuiCol_SeparatorActive] = borderColor;
      colors[ImGuiCol_ResizeGrip] = bgColor;
      colors[ImGuiCol_ResizeGripHovered] = panelColor;
      colors[ImGuiCol_ResizeGripActive] = lightBgColor;
      colors[ImGuiCol_PlotLines] = panelActiveColor;
      colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
      colors[ImGuiCol_PlotHistogram] = panelActiveColor;
      colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
      colors[ImGuiCol_ModalWindowDarkening] = bgColor;
      colors[ImGuiCol_DragDropTarget] = bgColor;
      colors[ImGuiCol_NavHighlight] = bgColor;
      colors[ImGuiCol_DockingPreview] = panelActiveColor;
      colors[ImGuiCol_Tab] = bgColor;
      colors[ImGuiCol_TabActive] = panelActiveColor;
      colors[ImGuiCol_TabUnfocused] = bgColor;
      colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
      colors[ImGuiCol_TabHovered] = panelHoverColor;

      style.WindowRounding = 0.0f;
      style.ChildRounding = 0.0f;
      style.FrameRounding = 0.0f;
      style.GrabRounding = 0.0f;
      style.PopupRounding = 0.0f;
      style.ScrollbarRounding = 0.0f;
      style.TabRounding = 0.0f;
    }

    void uploadFonts() {
      VkCommandBuffer commandBuffer = renderer->commandPool.beginSingleTimeCommands();
      ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
      renderer->commandPool.endSingleTimeCommands(renderer->context.device.graphicsQueue, commandBuffer);
      ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    void newScene() {
      memory::Allocator::Permanent()->clear();
      core::Core::Instance->get<scene::Manager>()->reset();
    }

    std::filesystem::path resourceBase;

    render::vulkan::VulkanRenderer* renderer;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    uint32_t queueFamily = (uint32_t)-1;

    AssetBrowser meshBrowser;

    AssetBrowser textureBrowser;

    SceneBrowser sceneBrowser;
    
    SceneSave sceneSave;


    bool showDemoWindow = false;
  };
}