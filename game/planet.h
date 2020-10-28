#ifndef PLANET_H
#define PLANET_H

#include "engine.h"
#include "debug.h"

using namespace wage;

#define PlanetComponent 2002

void addRandomPlanet(scene::Scene& scene);

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
      auto manager = core::Core::Instance->get<scene::Manager>();
      addRandomPlanet(manager->currentScene());
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
    auto& entities = scene::Scene::current().entities();
    for (auto planent : entities.with({PlanetComponent})) {
      if (!planent.valid()) {
        continue;
      }
      planent.get<math::Transform>(TransformComponent)->localRotation.rotated(4, math::Vector3::Up);
    }
  }
};

void addPlanet(scene::Scene& scene, math::Vector position, float scale, int texture) {
  auto entity = IDCHECK(scene.entities().create());
  auto transform = entity.assign<math::Transform>(TransformComponent);
  transform->localPosition = position;
  transform->localScale = math::Vector(scale, scale, scale);
  entity.assign<physics::RigidBody>(RigidBodyComponent, 0.01, physics::RigidBody::Type::immovable);
  entity.assign<render::MeshSpec>(MeshComponent, render::MeshSpec::Sphere);
  entity.assign<physics::Collider>(ColliderComponent, physics::Collider::Type::sphere);
  if (texture == 0) {
    entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("earthlike_planet.png"));
  } else {
    entity.assign<render::MaterialSpec>(MaterialComponent, render::TextureSpec("test_planet.png"));
  }

  entity.assign<Planet>(PlanetComponent);
}

float randomComp() {
  auto comp = randomBetween(-20000, 20000);
  if (comp > 0) comp += 3000;
  if (comp < 0) comp -= 3000;
  return comp;
}

void addRandomPlanet(scene::Scene& scene) {
  float x = randomComp();
  float y = randomComp();
  float z = randomComp();
  float scale = randomBetween(2000, 2800);
  int texture = rand() % 2;
  addPlanet(scene, math::Vector(x, y, z), scale, texture);
}

#endif // PLANET_H