#include <iostream>

#include "ecs/registry.h"
#include "ecs/system/func_system.h"

using namespace wage;

class Comp {
public:
  static std::string name;

  Comp() : Comp(-1) {}

  Comp(int val) : val(val) {
  }

  void test() {
    printf("Tested -> %d \n", val);
  }

  int val;
};
std::string Comp::name = "test";

class Comp2 {
public:
  static std::string name;

  void test() {
    printf("Tested2!\n");
  }
};
std::string Comp2::name = "test2";


int main(int argc, char* argv[]) {

  // ObjectPool<Comp> pool;
  // pool.create(3);
  // // pool.create();
  // for(auto it : pool) {
  //   printf("Ok!\n");
  //   it->test();
  // }

  Registry registry;


  for (int i = 0; i < 100; ++i) {
    auto entity = registry.create();
    entity.assign<Comp>(i);
    // entity.assign<Comp2>();
    entity.get<Comp>()->test();
    // entity.get<Comp2>()->test();
  }

  // for(auto it : registry.with<Comp>()) {
  //   printf("ID: %d\n", it.id());
  //   it.get<Comp>()->test();
  // }

  auto sys = FuncSystem();
} 