
#include <unistd.h>
#include <iostream>

#include "engine.h"

using namespace wage;

int main(int argc, char* argv[]) {

  ecs::EntityManager manager;

  manager.with({1, 12});
  std::cout << "Got 'ere\n";
}