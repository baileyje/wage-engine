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

  void update() {
    // printf("Comp Called!\n");
  }

  void fixedUpdate() {
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
  entity.getTransform()->setPosition(Vector(20.0, 20.0, 0.0));
  entity.getTransform()->setScale(Vector(20.0, 20.0));
  entity.add(new DorkComp());
  core.add(&entity);

  Entity entity2;
  entity2.getTransform()->setPosition(Vector(200.0, 200.0, 0.0));
  entity2.getTransform()->setScale(Vector(25.0, 25.0));
  entity2.getTransform()->setRotation(Vector(0.0, 0.0, 3.6));
  entity2.add(new DorkComp());
  core.add(&entity2);

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