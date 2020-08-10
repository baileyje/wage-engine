#include <iostream>

#include "new_ecs/common.h"
#include "new_ecs/component_pool.h"
#include "new_ecs/registry.h"
#include "new_ecs/manager.h"
#include "memory/indexed_storage.h"
#include "memory/pool.h"

using namespace wage::ecs;
using namespace wage::memory;

#define ECS_ENT_ID_TYPE uint16
#define ECS_ENT_VER_TYPE uint16

struct Position {
  int64_t x;
  int64_t y;
  int64_t z;
};

struct Player {
  std::string name;
};

int main(int argc, char* argv[]) {

  ComponentType PositionComponent = 0;
  ComponentType PlayerComponent = 1;

  EntityManager manager;

  manager.registerComponent(PositionComponent, sizeof(Position));
  manager.registerComponent(PlayerComponent, sizeof(Player));

  auto player = manager.create();
  printf("%hu\n", player.id().id());
  player.set<Position>(PositionComponent, {0, 0, 0});
  player.set<Player>(PlayerComponent, {"Frank the Tank.."});


  auto player2 = manager.create();
  printf("%hu\n", player2.id().id());
  // player2.set<Position>(PositionComponent, {0, 0, 0});
  player2.set<Player>(PlayerComponent, {"Frank the Stank.."});

  // Add some fake components to some entities
  for (int entId = 0; entId < 10; entId++) {
    auto entity = manager.create();
    auto comp = entity.set<Position>(PositionComponent, {0, entId, 0});
    // printf("%i: %s\n", entity.id(), comp->test.c_str());
  }
  printf("Num positions: %zu\n", manager.registry().componentCount(PositionComponent));
  printf("Num players: %zu\n", manager.registry().componentCount(PlayerComponent));

  // auto playerPool = manager.registry().pool(PlayerComponent);
  // for (auto item : *playerPool) {
  //   printf("Player Item: %i\n", item.entityId());
  // }
  // auto posPool = manager.registry().pool(PositionComponent);
  // for (auto item : *posPool) {
  //   printf("Pos Item: %i\n", item.entityId());
  // }
  
  // auto withPlayer = manager.with({PlayerComponent});
  // for (auto entity : withPlayer) {
  //   printf("With Player: %i\n", entity.id().id());
  // }
  // auto withPosition = manager.with({PositionComponent});
  // for (auto entity : withPosition) {
  //   printf("With Pos: %i\n", entity.id().id());
  // }
  auto withBoth = manager.with({PlayerComponent, PositionComponent});
  for (auto entity : withBoth) {
    printf("With Both: %i\n", entity.id().id());
  }

  // auto playerAgain = manager.get(0);
  // auto pos = playerAgain.get<Position>(PositionComponent);
  // printf("Player pos...: %lli:%lli:%lli\n", pos->x, pos->y, pos->z);

  // auto test = reg.getComponent(playerAgain.id(), PositionComponent);
  // printf("%hu\n", test.entityId());

  // manager.destroy(player);
  // std::cout << "Player Valid before update 1...: " << player.valid() << "\n";
  // manager.update();
  // std::cout << "Player Valid before update 2...: " << player.valid() << "\n";
  // manager.update();
  // std::cout << "Player Valid before update 3...: " << player.valid() << "\n";

  // auto other = manager.get(2);
  // other.destroy();
  // std::cout << "Valid:" << other.valid() << "\n";
  // reg.flushDestroys();
  // std::cout << "Valid:" << other.valid() << "\n";
}