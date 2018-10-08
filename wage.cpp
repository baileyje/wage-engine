#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>

#include "core/core.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "engine/engine.h"
#include "entity/entity.h"

class DorkComp : public Component {

  void update(Context* context) {
    // printf("Comp Called!\n");
  }

  void fixedUpdate(Context* context) {
    // printf("Comp Fixed Called!\n");
  }

};


Core* coreRef;

void intHandler(int);

// So far so dumb
int main(int argc, char* argv[]) {
  char buffer[255];
  std::string path = std::string(getcwd(buffer, sizeof(buffer)));
  printf("Path: %s\n", path.c_str());
  signal(SIGINT, intHandler);  
  Core core(path);  
  coreRef = &core;
  Jsrt jsrt;
  core.add(&jsrt);
  // core.add(new Physics());
  
  Engine engine;
  core.add(&engine);

  core.add(new Renderer());

  Entity entity;
  entity.add(new DorkComp());
  engine.add(&entity);

  // FULL NONSENSE HERE!
  core.init();
  // jsrt->loadModule("boot.js");
  core.start();
  return 0;
}

void intHandler(int) { 
  if (coreRef != NULL) {
    coreRef->stop();
  }  
}