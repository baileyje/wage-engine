#include <iostream>

#include "util/sparse_set.h"
#include "memory/pool.h"

#include "ecs/entity_manager.h"
#include "ecs/system_manager.h"
#include "engine.h"

using namespace wage;

#define ECS_ENT_ID_TYPE uint16
#define ECS_ENT_VER_TYPE uint16

class TestSystem: public ecs::System {
  void update(const ecs::SystemContext& context) {
    printf("Yeah Boi\n");
  }
};

int main(int argc, char* argv[]) {

  core::Core::Instance->create<ecs::EntityManager>();
  core::Core::Instance->create<ecs::SystemManager>();
  core::Core::Instance->init();

  auto systemManager = core::Core::Instance->get<ecs::SystemManager>();
  auto entityManager = core::Core::Instance->get<ecs::EntityManager>();

  for (int i = 0; i < 10000; i++ ) {
    auto entity = entityManager->create();
    entity.assign<int>(10000 - i);
  }
  for (auto item : entityManager->with<int>()) {
    std::cout << "ViewEnt:" << item.id() << ":" << item.get<int>() << std::endl;
  }

  ecs::SystemManager sm;

  sm.create<TestSystem>();
}