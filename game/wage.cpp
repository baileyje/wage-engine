#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "engine.h"
#include "physics-bullet/physics.h"
#include "render-gl/renderer.h"
#include "render-vulkan/vulkan_renderer.h"
#include "platform-glfw/platform.h"
#include "input-glfw/input.h"
#include "audio-al/audio.h"

#include "player/player.h"
#include "player/cannon.h"
#include "enemy.h"
#include "wall.h"
#include "planet.h"
#include "dock.h"
#include "camera.h"
#include "hud.h"
#include "music.h"

using namespace wage;

void setupServices(core::Core* core, std::string path) {
  core->create<job::Manager>();
  auto fileSystem = core->create<fs::FileSystem, fs::Local>(path);
  core->create<asset::Manager, asset::FsManager>(fileSystem);
  core->create<messaging::Messaging>();
  core->create<platform::Platform, platform::GlfwPlatform>();
  core->create<input::Input, input::GlfwInput>();
  core->create<audio::Audio, audio::AlAudio>();
  core->create<physics::Physics, physics::BulletPhysics>();
  // core->create<render::Renderer, render::GlRenderer>();
  core->create<render::Renderer, render::VulkanRenderer>();
  core->create<scene::Manager>();
}

void setupCoreSystems(scene::Scene& scene) {
  scene.systems().create<ui::UiSystem>();
  scene.systems().create<render::MeshRenderer>();
  scene.systems().create<EnemyLauncher>();
  scene.systems().create<EnemyMovement>();
  // scene.systems().create<PlayerBasicMovement>();
  scene.systems().create<PlayerPhysicsMovement>();
  scene.systems().create<CannonControl>();
  scene.systems().create<PlanetLauncher>();
  scene.systems().create<DumbMusicSystem>();
  scene.systems().create<WallSystem>();
}

void registerKnownComponents(scene::Scene& scene) {
  // TODO: Make registering components not the effing worst.
  scene.entities().registerComponent(TransformComponent, sizeof(math::Transform));

  // Camera
  scene.entities().registerComponent(CameraComponent, sizeof(render::Camera));
  // entityManager->registerComponent(OrthographicCameraComponent, sizeof(render::OrthographicCamera));
  scene.entities().registerComponent(PerspectiveCameraComponent, sizeof(render::PerspectiveCamera));

  // Lights
  scene.entities().registerComponent(SpotlightComponent, sizeof(render::Spotlight));
  scene.entities().registerComponent(PointLightComponent, sizeof(render::PointLight));
  scene.entities().registerComponent(DirectionalLightComponent, sizeof(render::DirectionalLight));

  // Renderer
  scene.entities().registerComponent(MeshComponent, sizeof(render::MeshSpec));
  scene.entities().registerComponent(MaterialComponent, sizeof(render::MaterialSpec));

  // Physics
  scene.entities().registerComponent(ColliderComponent, sizeof(physics::Collider));
  scene.entities().registerComponent(RigidBodyComponent, sizeof(physics::RigidBody));

  // UI
  scene.entities().registerComponent(UiLabelComponent, sizeof(ui::UiLabel));

  // Game Junk
  scene.entities().registerComponent(PlayerComponent, sizeof(Player));
  scene.entities().registerComponent(EnemyComponent, sizeof(Enemy));
  scene.entities().registerComponent(PlanetComponent, sizeof(Planet));
  scene.entities().registerComponent(DockComponent, sizeof(Dock));
  scene.entities().registerComponent(CannonComponent, sizeof(Cannon));
  scene.entities().registerComponent(CannonBallComponent, sizeof(CannonBall));
  scene.entities().registerComponent(WallComponent, sizeof(Wall));
}

void setupScene(scene::Scene& scene) {

  registerKnownComponents(scene);

  setupCoreSystems(scene);
  // auto topLightEnt = entityManager->create();
  // topLightEnt.assign<math::Transform>(TransformComponent)->rotation(math::Vector(-90, 0, 0));
  // auto topLight = topLightEnt.assign<render::DirectionalLight>(DirectionalLightComponent);
  // topLight->diffuse(component::Color(0.7, 0.7, 0.7, 1));
  // topLight->ambient(component::Color(0.4, 0.4, 0.4, 1));

  // auto bottomLightEnt = entityManager->create();
  // bottomLightEnt.assign<math::Transform>(TransformComponent)->rotation(math::Vector(90, 0, 0));
  // auto bottomLight = bottomLightEnt.assign<render::DirectionalLight>(DirectionalLightComponent);
  // bottomLight->diffuse(component::Color(0.7, 0.7, 0.9, 1));
  // bottomLight->ambient(component::Color(0.4, 0.4, 0.4, 1));

  // auto launchPadEnt = entityManager->create();
  // launchPadEnt.assign<math::Transform>(TransformComponent, Vector(0, -15, 75), Vector(200, 15, 800), Vector(0, 0, 0));
  // launchPadEnt.assign<render::MeshSpec>(MeshComponent, render::MeshSpec::Cube);
  // launchPadEnt.assign<physics::RigidBody>(RigidBodyComponent, 1, physics::RigidBodyType::immovable);
  // launchPadEnt.assign<physics::Collider>(ColliderComponent, physics::ColliderType::box);
  // launchPadEnt.assign<render::MaterialSpec>(MaterialComponent, component::Color::White);

  addPlayer(scene);

  // addEnemy(scene, {0, 0, 20}, 5 /*scale*/);

  // addPlanet(scene, {100, 0, 1200}, 1000, 0);

  addCamera(scene);

  setupHud(scene);

  addWall(scene);

  // for (int i = 0; i < 200; i++) {
  //   addRandomEnemy(scene);
  // }
  // for (int i = 0; i < 10; i++) {
  //   addRandomPlanet(scene);
  // }
  // for (int i = 0; i < 5; i++) {
  //   addRandomDock(scene);
  // }
}

class GameController : public messaging::MessageListener<input::KeyEvent> {

  /**
   * Message listener triggered by entities being removed from the game.
   */
  inline bool on(const input::KeyEvent& event) {

    if (event.key() == input::Key::p && event.type() == input::KeyEventType::press) {
      core::Core::Instance->pause();
    }
    if (event.key() == input::Key::u && event.type() == input::KeyEventType::press) {
      core::Core::Instance->unpause();
    }
    if (event.key() == input::Key::f2 && event.type() == input::KeyEventType::press) {
      core::Core::Instance->reset();
    }
    return false;
  }
};

int main(int argc, char* argv[]) {

  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  signal(SIGINT, [](int _) {
    core::Core::Instance->stop();
  });

  setupServices(core::Core::Instance, path);
  auto sceneManager = core::Core::Instance->get<scene::Manager>();
  auto scene = sceneManager->currentScene();
  // setupScene(scene);
  sceneManager->builder(setupScene);
  core::Core::Instance->init({1.0 / 60.0});

  GameController controller;
  core::Core::Instance->get<messaging::Messaging>()->listen<input::KeyEvent>(&controller);

  core::Core::Instance->start();
  return 0;
}
