#include "signal.h"
#include <unistd.h>
#include <iostream>
#include <string>

#include "core/core.h"
#include "platform/platform.h"
#include "input/input.h"
#include "jsrt/jsrt.h"
#include "physics/physics.h"
#include "render/renderer.h"
#include "engine/engine.h"
#include "entity/entity.h"
#include "entity/component.h"

class DorkComp : public Component {

  void update(EntityContext* context) {
    if (Input::isPressed(GLFW_KEY_W)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
    }
    if (Input::isPressed(GLFW_KEY_S)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, -0.3);
    } 
    if (Input::isPressed(GLFW_KEY_A)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0.3, 0, 0);
    }
    if (Input::isPressed(GLFW_KEY_D)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(-0.3, 0, 0);
    }
    if (Input::isPressed(GLFW_KEY_SPACE)) {
      *context->getEntity()->getTransform()->getPosition() += Vector(0, 0.5, 0);
    }
  }

  void fixedUpdate(EntityContext* context) {
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
  core.add(new Platform());
  core.add(new Input());
  // Jsrt jsrt;
  // core.add(&jsrt);
  // core.add(new Physics());
  
  Engine engine;
  core.add(&engine);
  core.add(new Renderer());

  core.getCamera()->getTransform()->setPosition(Vector(0, 5, -20));
  core.getCamera()->getTransform()->setRotation(Vector(0.0, 0.0, 0));

  // for (int i = 0; i < 5; i++) {
  //   {
  //     Entity* entity = new Entity();
  //     entity->getTransform()->setPosition(Vector(3 * i,10 + 5 * i, 0));
  //     printf("X: %f : Y: %f, Z: %f\n", 
  //       entity->getTransform()->getPosition()->x,
  //       entity->getTransform()->getPosition()->y,
  //       entity->getTransform()->getPosition()->x
  //     );
  //     entity->getTransform()->setScale(Vector(1, 1, 1));
  //     // entity->getTransform()->setRotation(Vector(0.2, 0.2, 0.2));
  //     entity->add(new DorkComp());
  //     core.add(entity);
  //   }
  // }

  Entity* entity = new Entity();
  entity->getTransform()->setPosition(Vector(0, 2, 0));
  entity->getTransform()->setScale(Vector(1, 1, 1));
  entity->getTransform()->setRotation(Vector(0, 0, 0));
  entity->add(new DorkComp());
  core.add(entity);
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