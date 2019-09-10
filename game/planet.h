#ifndef PLANET_H
#define PLANET_H

#include "engine.h"

using namespace wage;

void addRandomPlanet(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager);

class Planet {
};

class PlanetLauncher : public ecs::System {

public:
  PlanetLauncher() : System() {
  }

  void start(const ecs::SystemContext& context) {
    lastLaunch = context.time();
  }

  void update(const ecs::SystemContext& context) {
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::p) && context.time() - lastLaunch > launchThreshold) {
      auto manager = core::Core::Instance->get<ecs::EntityManager>();
      auto systemManager = core::Core::Instance->get<ecs::SystemManager>();
      addRandomPlanet(manager, systemManager);
      lastLaunch = context.time();
    }
  }

private:

  double lastLaunch = 0;

  double launchThreshold = 0.001;

};

class PlanetRotation : public ecs::System {
public:

  PlanetRotation() : System() {
  }

  void fixedUpdate(const ecs::SystemContext& context) {
    auto manager = core::Core::Instance->get<ecs::EntityManager>();
    for (auto entity : manager->with<Planet>()) {
      if (!entity.valid()) {
        continue;
      }
      entity.get<math::Transform>()->rotation().rotated(4, math::Vector3::Up);
    }
  }
};

void addPlanet(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager, math::Vector position, float scale) {
  Entity entity = entityManager->create();
  auto transform = entity.assign<math::Transform>();
  transform->position(position);
  transform->localScale(math::Vector(scale, scale, scale));
  entity.assign<physics::RigidBody>(0.01, physics::RigidBodyType::immovable);
  entity.assign<render::Mesh>(render::Mesh::Sphere);
  entity.assign<physics::Collider>(physics::ColliderType::sphere);
  entity.assign<render::Material>(render::Texture("textures/earthlike_planet.png"));
  entity.assign<Planet>();
}

void addRandomPlanet(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  float x = rand() % 5000 - 2500;
  float y = rand() % 5000 - 2500;
  float z = rand() % 5000 - 2500;
  // float scale = (rand() % 100) / 30.0;
  addPlanet(entityManager, systemManager, math::Vector(x, y, z) * 2, 100);
}

#endif // PLANET_H