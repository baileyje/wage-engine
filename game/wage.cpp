#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

#include "engine.h"
#include "physics-bullet/physics.h"
#include "render-gl/renderer.h"
#include "platform-glfw/platform.h"
#include "input-glfw/input.h"
#include "audio-al/audio.h"

#include "player/player.h"
#include "player/cannon.h"
#include "enemy.h"
#include "planet.h"
#include "dock.h"
#include "camera.h"
#include "hud.h"
#include "music.h"

using namespace wage;

void setupServices(core::Core* core, std::string path) {
  auto fileSystem = core->create<fs::FileSystem, fs::Local>(path);
  core->create<assets::Manager, assets::FsManager>(fileSystem);
  core->create<messaging::Messaging>();
  core->create<platform::Platform, platform::GlfwPlatform>();
  core->create<input::Input, input::GlfwInput>();
  core->create<audio::Audio, audio::AlAudio>();
  core->create<ecs::EntityManager>();
  core->create<ecs::SystemManager>();
  core->create<physics::Physics, physics::BulletPhysics>();
  core->create<render::Renderer, render::GlRenderer>();
}

void setupCoreSystems(ecs::SystemManager* systemManager) {
  systemManager->create<ui::UiSystem>();
  systemManager->create<render::MeshRenderer>();
  systemManager->create<EnemyLauncher>();
  systemManager->create<EnemyMovement>();
  // systemManager->create<PlayerBasicMovement>();
  systemManager->create<PlayerPhysicsMovement>();
  systemManager->create<CannonControl>();
  systemManager->create<PlanetLauncher>();
  systemManager->create<DumbMusicSystem>();
}

void registerKnownComponents(ecs::EntityManager* entityManager) {
  // TODO: Move engine provided components to the some engine bootstap function.

  entityManager->registerComponent(TransformComponent, sizeof(math::Transform));

  // Camera
  entityManager->registerComponent(CameraComponent, sizeof(render::Camera));
  // entityManager->registerComponent(OrthographicCameraComponent, sizeof(render::OrthographicCamera));
  entityManager->registerComponent(PerspectiveCameraComponent, sizeof(render::PerspectiveCamera));

  // Lights
  entityManager->registerComponent(SpotlightComponent, sizeof(render::Spotlight));
  entityManager->registerComponent(PointLightComponent, sizeof(render::PointLight));
  entityManager->registerComponent(DirectionalLightComponent, sizeof(render::DirectionalLight));

  // Renderer
  entityManager->registerComponent(MeshComponent, sizeof(render::MeshSpec));
  entityManager->registerComponent(MaterialComponent, sizeof(render::MaterialSpec));

  // Physics
  entityManager->registerComponent(ColliderComponent, sizeof(physics::Collider));
  entityManager->registerComponent(RigidBodyComponent, sizeof(physics::RigidBody));

  // UI
  entityManager->registerComponent(UiLabelComponent, sizeof(ui::UiLabel));

  // Game Junk
  entityManager->registerComponent(PlayerComponent, sizeof(Player));
  entityManager->registerComponent(EnemyComponent, sizeof(Enemy));
  entityManager->registerComponent(PlanetComponent, sizeof(Planet));
  entityManager->registerComponent(DockComponent, sizeof(Dock));
  entityManager->registerComponent(CannonComponent, sizeof(Cannon));
  entityManager->registerComponent(CannonBallComponent, sizeof(CannonBall));
}

void setupScene(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
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

  addPlayer(entityManager, systemManager);

  addEnemy(entityManager, systemManager, {0, 0, 20}, 5 /*scale*/);

  addPlanet(entityManager, systemManager, {100, 0, 1200}, 1000, 0);

  addCamera(entityManager, systemManager);

  setupHud(entityManager, systemManager);

  for (int i = 0; i < 200; i++) {
    addRandomEnemy(entityManager, systemManager);
  }
  // for (int i = 0; i < 10; i++) {
  //   addRandomPlanet(entityManager, systemManager);
  // }
  // for (int i = 0; i < 5; i++) {
  //   addRandomDock(entityManager, systemManager);
  // }
}

int main(int argc, char* argv[]) {

  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  signal(SIGINT, [](int _) {
    core::Core::Instance->stop();
  });

  setupServices(core::Core::Instance, path);
  auto systemManager = core::Core::Instance->get<ecs::SystemManager>();
  auto entityManager = core::Core::Instance->get<ecs::EntityManager>();
  registerKnownComponents(entityManager);
  setupCoreSystems(systemManager);

  setupScene(entityManager, systemManager);
  core::Core::Instance->init();
  core::Core::Instance->start();
  return 0;
}
