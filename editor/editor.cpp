#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <filesystem>

#include "engine.h"
#include "physics-bullet/physics.h"
#include "render-gl/renderer.h"
#include "render-vulkan/vulkan_renderer.h"
#include "platform-glfw/platform.h"
#include "input-glfw/input.h"
#include "audio-al/audio.h"
#include "scene-serializer/serializer.h"

#include "camera.h"
#include "edit_services.h"
#include "gui.h"

using namespace wage;
using namespace wage::editor;

void setupServices(core::Core* core, std::string path) {
  core->create<job::Manager>();
  auto fileSystem = core->create<fs::FileSystem, fs::Local>(path);
  core->create<asset::Manager, asset::FsManager>(fileSystem);
  core->create<messaging::Messaging>();
  core->create<platform::Platform, platform::GlfwPlatform>();
  core->create<input::Input, input::GlfwInput>(false);
  core->create<audio::Audio, audio::AlAudio>();
  core->create<render::Renderer, render::vulkan::VulkanRenderer>();
  core->create<scene::Manager>();

  core->create<serialize::SceneSerializer>();

  // Editor specific services
  core->create<EditorController>();
  core->create<EditorGizmos>();
  core->create<CameraController>();
  // core->create<EditorSelectionController>();
  core->create<SceneController>();

  core->create<Gui>(std::filesystem::path(path) / "resources");
}

void setupCoreSystems(scene::Scene& scene) {
  scene.systems().create<ui::UiSystem>();
  scene.systems().create<render::MeshRenderer>();
}

void registerKnownComponents(scene::Scene& scene) {
  //   // TODO: Make registering components not the effing worst.s
  scene.entities().registerComponent(TransformComponent, sizeof(math::Transform));
  
  //   // Camera
  //   scene.entities().registerComponent(CameraComponent, sizeof(render::Camera));
  //   // entityManager->registerComponent(OrthographicCameraComponent, sizeof(render::OrthographicCamera));
  scene.entities().registerComponent(PerspectiveCameraComponent, sizeof(render::PerspectiveCamera));

  //   // Lights
  //   scene.entities().registerComponent(SpotlightComponent, sizeof(render::Spotlight));
  //   scene.entities().registerComponent(PointLightComponent, sizeof(render::PointLight));
  //   scene.entities().registerComponent(DirectionalLightComponent, sizeof(render::DirectionalLight));

  //   // Renderer
  scene.entities().registerComponent(MeshComponent, sizeof(render::MeshSpec));
  scene.entities().registerComponent(MaterialComponent, sizeof(render::MaterialSpec));

  //   // Physics
  scene.entities().registerComponent(ColliderComponent, sizeof(physics::Collider));
  scene.entities().registerComponent(RigidBodyComponent, sizeof(physics::RigidBody));

  scene.entities().registerComponent(NoSerializeComponent, sizeof(serialize::NoSerialize));
}

void setupScene(scene::Scene& scene) {

  registerKnownComponents(scene);
  setupCoreSystems(scene);
  addCamera(scene);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, [](int _) {
    core::Core::Instance->stop();
  });
  std::filesystem::path path = std::filesystem::path("..") / "game";
  setupServices(core::Core::Instance, std::filesystem::canonical(path));
  auto sceneManager = core::Core::Instance->get<scene::Manager>();
  auto scene = sceneManager->currentScene();
  // setupScene(scene);
  sceneManager->builder(setupScene);

  // GameController controller;
  // core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(&controller);
  core::Core::Instance->init({1.0 / 60.0});
  core::Core::Instance->start();

  return 0;
}
