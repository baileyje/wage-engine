#include <iostream>

#include "util/sparse_set.h"
#include "memory/pool.h"

#include "new_ecs/entity_manager.h"
#include "new_ecs/system_manager.h"
#include "engine.h"

using namespace wage;

#define ECS_ENT_ID_TYPE uint16
#define ECS_ENT_VER_TYPE uint16

class TestSystem: public System {
  void update(const SystemContext& context) {
    printf("Yeah Boi\n");
  }
};

int main(int argc, char* argv[]) {

  Core::Instance->create<EntityManager>();
  Core::Instance->create<SystemManager>();
  Core::Instance->init();

  auto systemManager = Core::Instance->get<SystemManager>();
  auto entityManager = Core::Instance->get<EntityManager>();

  for (int i = 0; i < 10000; i++ ) {
    auto entity = entityManager->create();
    entity.assign<int>(10000 - i);
  }
  for (auto item : entityManager->with<int>()) {
    std::cout << "ViewEnt:" << item.id() << ":" << item.get<int>() << std::endl;
  }

  SystemManager sm;

  sm.create<TestSystem>();
}