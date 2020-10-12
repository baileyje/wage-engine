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

#include "camera.h"
#include "edit_services.h"
#include "gui.h"

// #include "player/player.h"
// #include "player/cannon.h"
// #include "enemy.h"
// #include "wall.h"
// #include "planet.h"
// #include "dock.h"
// #include "hud.h"
// #include "music.h"
// #include "tmp.h"

using namespace wage;
using namespace wage::editor;

void setupServices(core::Core* core, std::string path) {
  wage::core::Logger::info("Path: ", path);
  core->create<job::Manager>();
  auto fileSystem = core->create<fs::FileSystem, fs::Local>(path);
  core->create<asset::Manager, asset::FsManager>(fileSystem);
  core->create<messaging::Messaging>();
  core->create<platform::Platform, platform::GlfwPlatform>();
  core->create<input::Input, input::GlfwInput>(false);
  core->create<audio::Audio, audio::AlAudio>();
  // core->create<physics::Physics, physics::BulletPhysics>();
  // core->create<render::Renderer, render::GlRenderer>();
  core->create<render::Renderer, render::vulkan::VulkanRenderer>();
  core->create<scene::Manager>();

  // Editor specific services
  core->create<EditorController>();
  core->create<EditorGizmos>();
  core->create<CameraController>();
  core->create<Gui>( std::filesystem::path(path) / "resources");
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
  //   scene.entities().registerComponent(RigidBodyComponent, sizeof(physics::RigidBody));

  //   // UI
  //   scene.entities().registerComponent(UiLabelComponent, sizeof(ui::UiLabel));

  //   // Game Junk
  //   scene.entities().registerComponent(PlayerComponent, sizeof(Player));
  //   scene.entities().registerComponent(EnemyComponent, sizeof(Enemy));
  //   scene.entities().registerComponent(PlanetComponent, sizeof(Planet));
  //   scene.entities().registerComponent(DockComponent, sizeof(Dock));
  //   scene.entities().registerComponent(CannonComponent, sizeof(Cannon));
  //   scene.entities().registerComponent(CannonBallComponent, sizeof(CannonBall));
  //   scene.entities().registerComponent(WallComponent, sizeof(Wall));
}

void setupScene(scene::Scene& scene) {

  registerKnownComponents(scene);
  setupCoreSystems(scene);
  addCamera(scene);

  // for (int i = 0; i < 1; ++i) {
  //   auto entity = scene.entities().create();
  //   auto transform = entity.assign<math::Transform>(TransformComponent);
  //   transform->localPosition = {static_cast<float>(i * 6), 0, 0};
  //   transform->localScale = {1, 1, 1};
  //   entity.assign<render::MeshSpec>(MeshComponent, "cube.obj");
  //   entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("odd_space_2.png"));
  //   auto collider = entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
  //   collider->transform.localScale = {2, 2, 2};
  // }
  //   setupHud(scene);
}

// class GameController : public messaging::MessageListener<input::KeyEvent> {

//   /**
//    * Message listener triggered by entities being removed from the game.
//    */
//   inline bool on(const input::KeyEvent& event) {

//     if (event.key() == input::Key::p && event.type() == input::KeyEventType::press) {
//       core::Core::Instance->pause();
//     }
//     if (event.key() == input::Key::u && event.type() == input::KeyEventType::press) {
//       core::Core::Instance->unpause();
//     }
//     if (event.key() == input::Key::f2 && event.type() == input::KeyEventType::press) {
//       core::Core::Instance->reset();
//     }
//     if (event.key() == input::Key::f4 && event.type() == input::KeyEventType::press) {
//       auto renderer = core::Core::Instance->get<render::Renderer>();
//       renderer->renderWireFrames = !renderer->renderWireFrames;
//     }
//     return false;
//   }
// };

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
