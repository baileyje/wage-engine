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

#include "player.h"
#include "enemy.h"
#include "planet.h"
#include "camera.h"
#include "hud.h"

using namespace wage;


void setupServices(core::Core* core, std::string path) {
  auto fileSystem = core->create<fs::FileSystem, fs::Local>(path);
  core->create<assets::Manager, assets::FsManager>(fileSystem);
  core->create<messaging::Messaging>();
  core->create<platform::Platform, platform::GlfwPlatform>();
  core->create<input::Input, input::GlfwInput>();
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
  systemManager->create<PlayerMovement>();
  systemManager->create<PlanetLauncher>();
}

void setupScene(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  auto topLightEnt = entityManager->create();
  topLightEnt.assign<math::Transform>()->rotation(math::Vector(-90, 0, 0));
  auto topLight = topLightEnt.assign<component::DirectionalLight>();
  topLight->diffuse(math::Color(0.7, 0.7, 0.7, 1));
  topLight->ambient(math::Color(0.4, 0.4, 0.4, 1));

  auto bottomLightEnt = entityManager->create();
  bottomLightEnt.assign<math::Transform>()->rotation(math::Vector(90, 0, 0));
  auto bottomLight = bottomLightEnt.assign<component::DirectionalLight>();
  bottomLight->diffuse(math::Color(0.7, 0.7, 0.9, 1));
  bottomLight->ambient(math::Color(0.4, 0.4, 0.4, 1));

  addPlayer(entityManager, systemManager);

  addCamera(entityManager, systemManager);

  setupHud(entityManager, systemManager);

  for (int i = 0; i < 200; i++) {
    addRandomEnemy(entityManager, systemManager);
  }
  for (int i = 0; i < 10; i++) {
    addRandomPlanet(entityManager, systemManager);
  }
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
  setupCoreSystems(systemManager);

  render::Mesh::generatePrimitives();
  setupScene(entityManager, systemManager);
  core::Core::Instance->init();
  core::Core::Instance->start();

  return 0;
}
