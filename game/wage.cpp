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
#include "camera.h"
#include "hud.h"

using namespace wage;


void setupServices(Core* core, std::string path) {
  auto fileSystem = core->create<FileSystem, LocalFileSystem>(path);
  core->create<AssetManager, FsAssetManager>(fileSystem);
  core->create<Messaging>();
  core->create<Platform, GlfwPlatform>();
  core->create<Input, GlfwInput>();
  core->create<EntityManager>();
  core->create<SystemManager>();
  core->create<Physics, BulletPhysics>();
  core->create<Renderer, GlRenderer>();
}

void setupCoreSystems(SystemManager* systemManager) {
  systemManager->create<UiSystem>();
  systemManager->create<MeshRenderer>();
  systemManager->create<EnemyMovement>();
  systemManager->create<PlayerMovement>();
}

void setupScene(EntityManager* entityManager, SystemManager* systemManager) {
  auto topLightEnt = entityManager->create();
  topLightEnt.assign<Transform>()->rotation(Vector(-90, 0, 0));
  auto topLight = topLightEnt.assign<DirectionalLight>();
  topLight->diffuse(Color(0.7, 0.7, 0.7, 1));
  topLight->ambient(Color(0.4, 0.4, 0.4, 1));

  auto bottomLightEnt = entityManager->create();
  bottomLightEnt.assign<Transform>()->rotation(Vector(90, 0, 0));
  auto bottomLight = bottomLightEnt.assign<DirectionalLight>();
  bottomLight->diffuse(Color(0.7, 0.7, 0.9, 1));
  bottomLight->ambient(Color(0.4, 0.4, 0.4, 1));

  auto player = addPlayer(entityManager, systemManager);

  auto cameraEntity = entityManager->create();
  auto camTransform = cameraEntity.assign<Transform>();
  camTransform->position(Vector(0, 30, -30));
  camTransform->rotation(Vector(00, 0.0, 0));
  cameraEntity.assign<PerspectiveCamera>();
  systemManager->create<ThirdPersonCamera>(cameraEntity, player);

  setupHud(entityManager, systemManager);

  for (int i = 0; i < 200; i++) {
    addRandomEnemy(entityManager, systemManager);
  }
}

int main(int argc, char* argv[]) {

  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  signal(SIGINT, [](int _) {
    Core::Instance->stop();
  });

  setupServices(Core::Instance, path);
  auto systemManager = Core::Instance->get<SystemManager>();
  auto entityManager = Core::Instance->get<EntityManager>();
  setupCoreSystems(systemManager);

  Mesh::generatePrimitives();
  setupScene(entityManager, systemManager);
  Core::Instance->init();
  Core::Instance->start();

  return 0;
}
