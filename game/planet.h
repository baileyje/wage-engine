#ifndef PLANET_H
#define PLANET_H

#include "engine.h"

using namespace wage;

void addRandomPlanet(EntityManager* entityManager, SystemManager* systemManager);

class Planet {
};

class PlanetLauncher : public System {

public:
  PlanetLauncher() : System() {
  }

  void start(const SystemContext& context) {
    lastLaunch = context.time();
  }

  void update(const SystemContext& context) {
    if (Core::Instance->get<Input>()->isPressed(Key::p) && context.time() - lastLaunch > launchThreshold) {
      auto manager = Core::Instance->get<EntityManager>();
      auto systemManager = Core::Instance->get<SystemManager>();
      addRandomPlanet(manager, systemManager);
      lastLaunch = context.time();
    }
  }

private:

  double lastLaunch = 0;

  double launchThreshold = 0.001;

};

class PlanetRotation : public System {
public:

  PlanetRotation() : System() {
  }

  void fixedUpdate(const SystemContext& context) {
    auto manager = Core::Instance->get<EntityManager>();
    for (auto entity : manager->with<Planet>()) {
      if (!entity.valid()) {
        continue;
      }
      entity.get<Transform>()->rotation().rotated(4, Vector3::Up);
    }
  }
};

void addPlanet(EntityManager* entityManager, SystemManager* systemManager, Vector position, float scale) {
  Entity entity = entityManager->create();
  auto transform = entity.assign<Transform>();
  transform->position(position);
  transform->localScale(Vector(scale, scale, scale));
  entity.assign<RigidBody>(0.01, RigidBodyType::immovable);
  entity.assign<Mesh>(Mesh::Sphere);
  entity.assign<Collider>(ColliderType::sphere);
  entity.assign<Material>(Texture("textures/earthlike_planet.png"));
  entity.assign<Planet>();
}

void addRandomPlanet(EntityManager* entityManager, SystemManager* systemManager) {
  float x = rand() % 5000 - 2500;
  float y = rand() % 5000 - 2500;
  float z = rand() % 5000 - 2500;
  // float scale = (rand() % 100) / 30.0;
  addPlanet(entityManager, systemManager, Vector(x, y, z) * 2, 100);
}

#endif // PLANET_H  