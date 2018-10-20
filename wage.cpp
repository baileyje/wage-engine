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
#include "entity/component/rigid_body.h"

class MoveIt : public Component {

public:

  MoveIt() : Component("MoveIt") {    
  }

  void update(EntityContext* context) {
    Entity* entity = context->getEntity();
    RigidBody* body = (RigidBody*)entity->getComponent("RigidBody");
    // body->addForce(Vector(0, 0.0000000001, 0));
    if (Input::isPressed(GLFW_KEY_W)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
      body->addForce(Vector(0, 0, 0.0004));
    }
    if (Input::isPressed(GLFW_KEY_S)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, -0.3);
      body->addForce(Vector(0, 0, -0.0004));
    } 
    if (Input::isPressed(GLFW_KEY_A)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0.3, 0, 0);
      body->addForce(Vector(0.0004, 0, 0));
    }
    if (Input::isPressed(GLFW_KEY_D)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(-0.3, 0, 0);
      body->addForce(Vector(-0.0004, 0, 0));
    }
    if (Input::isPressed(GLFW_KEY_SPACE)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0.5, 0);
      body->addForce(Vector(0, 0.0005, 0));
    }
    if (Input::isPressed(GLFW_KEY_LEFT_SHIFT)) {
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, -0.5, 0);
      body->addForce(Vector(0, -0.0001, 0));
    }
  }

  // void fixedUpdate(EntityContext* context) {
  //   // printf("Comp Fixed Called!\n");
  // }

};

class DorkComp2 : public Component {

public:

  DorkComp2() : Component("DorkComp2") {    
  }

  void update(EntityContext* context) {    
    if (Input::isPressed(GLFW_KEY_M)) {
      Entity* entity = context->getEntity();
      RigidBody* body = (RigidBody*)entity->getComponent("RigidBody");
      body->addForce(Vector(0, 0.0001, 0));
      // *context->getEntity()->getTransform()->getPosition() += Vector(0, 0, 0.3);
    }
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
  core.add(new Physics());
  
  Engine engine;
  core.add(&engine);
  core.add(new Renderer());

  // core.getCamera()->add(new DorkComp());
  // core.add(core.getCamera());

  core.getCamera()->getTransform()->setPosition(Vector(0, 5, -20));
  core.getCamera()->getTransform()->setRotation(Vector(0.0, 0.0, 0));

  for (int i = 0; i < 10; i++) {
      Entity* entity = new Entity();
      entity->getTransform()->setPosition(Vector(3, 3 * i, 0));   
      RigidBody* body = new RigidBody();
      body->mass = 0.001;
      entity->getTransform()->setScale(Vector(1, 1, 1));
      // entity->getTransform()->setRotation(Vector(0.2, 0.2, 0.2));
      entity->add(body);
      entity->add(new DorkComp2());
      core.add(entity);
  }

  Entity* mover = new Entity();
  mover->getTransform()->setPosition(Vector(0, 2, 0));
  mover->getTransform()->setScale(Vector(1, 1, 1));
  mover->getTransform()->setRotation(Vector(0, 0, 0));
  RigidBody* body = new RigidBody();
  body->mass = 0.001;
  mover->add(body);
  mover->add(new MoveIt());
  core.add(mover);

  Entity* ground = new Entity();
  ground->getTransform()->setPosition(Vector(0, -2, 0));
  ground->getTransform()->setScale(Vector(100, 0.1, 100));
  ground->getTransform()->setRotation(Vector(0, 0, 0));
  RigidBody* groundBody = new RigidBody();
  ground->add(groundBody);
  core.add(ground);

  core.init();
  core.start();
  return 0;
}

void intHandler(int) { 
  if (coreRef != NULL) {
    coreRef->stop();
  }  
}