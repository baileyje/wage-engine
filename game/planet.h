#ifndef PLANET_H
#define PLANET_H

#include "engine.h"

using namespace wage;

#define PlanetComponent 2002

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
    if (core::Core::Instance->get<input::Input>()->isPressed(input::Key::l) && context.time() - lastLaunch > launchThreshold) {
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
    for (auto planent : manager->with({PlanetComponent})) {
      if (!planent.valid()) {
        continue;
      }
      planent.get<math::Transform>(TransformComponent)->rotation().rotated(4, math::Vector3::Up);
    }
  }
};

void addPlanet(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager, math::Vector position, float scale, int texture) {
  auto entity = entityManager->create();
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->position(position);
  transform->localScale(math::Vector(scale, scale, scale));
  entity.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBodyType::immovable);
  entity.assign<render::MeshSpec>(MeshComponent, render::MeshSpec::Sphere);
  entity.assign<physics::Collider>(ColliderComponent, physics::ColliderType::sphere);
  if (texture == 0) {
    entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("earthlike_planet.png"));
  } else {
    entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("test_planet.png"));
  }

  entity.assign<Planet>(PlanetComponent);
}

void addRandomPlanet(ecs::EntityManager* entityManager, ecs::SystemManager* systemManager) {
  float x = randomBetween(-20000, 20000);
  float y = randomBetween(-20000, 20000);
  float z = randomBetween(-20000, 20000);
  float scale = randomBetween(800, 1600);
  int texture = rand() % 2;
  addPlanet(entityManager, systemManager, math::Vector(x, y, z), scale, texture);
}

#endif // PLANET_H